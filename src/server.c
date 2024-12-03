/**
 * Copyright (C) 2022 Carnegie Mellon University
 *
 * This file is part of the HTTP course project developed for
 * the Computer Networks course (15-441/641) taught at Carnegie
 * Mellon University.
 *
 * No part of the HTTP project may be copied and/or distributed
 * without the express permission of the 15-441/641 course staff.
 */
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "parse_http.h"
#include "ports.h"

#define BUF_SIZE 8192
// Closes a client's connection if they have not sent a valid request within
// CONNECTION_TIMEOUT seconds.
#define CONNECTION_TIMEOUT 50
#define MAX_CONNECTIONS 100

typedef struct {
    char *request_buffer;
    size_t buffer_size;
}http_context;

void reset_context(http_context *context) {
    if (context == NULL) {
        return;
    }

    // Free the dynamically allocated request buffer
    if (context->request_buffer) {
        free(context->request_buffer);
        context->request_buffer = NULL;
    }

    // Reset all fields to their initial state
    context->buffer_size = 0;
    //context->header_received = false;
}


void make_socket_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}


const char *get_mime_type(const char *path) {
    fprintf(stderr, "Get mime type\n");
    const char *ext = strrchr(path, '.'); // Get file extension
    if (!ext || ext == path)
        return OCTET_MIME; // Default for unknown or missing types

    ext++; // Move past the dot to get the extension without "."

    if (strcmp(ext, HTML_EXT) == 0)
        return HTML_MIME;
    if (strcmp(ext, CSS_EXT) == 0)
        return CSS_MIME;
    if (strcmp(ext, PNG_EXT) == 0)
        return PNG_MIME;
    if (strcmp(ext, JPG_EXT) == 0)
        return JPG_MIME;
    if (strcmp(ext, GIF_EXT) == 0)
        return GIF_MIME;

    return OCTET_MIME; // Default fallback
}

char *get_header_value(const Request *request, const char *header_name) {
    if (!request || !header_name) {
        return NULL;
    }

    for (int i = 0; i < request->header_count; ++i) {
        if (strcasecmp(request->headers[i].header_name, header_name) == 0) {
            return request->headers[i].header_value;
        }
    }

    return NULL; // Header not found
}

bool write_all(int client_fd, const char *buffer, size_t length) {
    size_t total_written = 0;
    while (total_written < length) {
        ssize_t bytes_written = write(client_fd, buffer + total_written, length - total_written);
        if (bytes_written > 0) {
            total_written += bytes_written;
        } else if (bytes_written == 0) {
            // Connection closed by client
            fprintf(stderr, "Connection closed by client\n");
            return false;
        } else {
            // bytes_written == -1, handle errors
            if (errno == EINTR) {
                // Interrupted, retry write
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Non-blocking mode, try again
                return false;
            } else {
                fprintf(stderr,"write\n");
                return false;
            }
        }
    }
    return true;
}

bool send_response(int client_fd, const char *status, const char *content_type,
                   const char *body, const char *last_modified) {

    char *response;
    size_t response_len;
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "%ld",
             body ? strlen(body) : 0);
    serialize_http_response(
            &response, &response_len, status, (char *) content_type, content_length,
            (char *) last_modified, body ? strlen(body) : 0, (char *) body);

    bool write_result = write_all(client_fd, response, response_len);
    free(response);
    return write_result;
}

bool handle_get_head(Request *request, int client_fd, const char *www_folder) {
    char full_path[BUF_SIZE];
    memset(full_path, 0, BUF_SIZE);
    if (strcmp(request->http_uri, "/") == 0) {
        // If the request is to the root, append /index.html
        snprintf(full_path, sizeof(full_path), "%s/index.html", www_folder);
    } else {
        // Otherwise, use the requested URI directly
        snprintf(full_path, sizeof(full_path), "%s%s", www_folder,
                 request->http_uri);
    }

    // fprintf(stderr, "%s\n", full_path);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        return send_response(client_fd, NOT_FOUND, HTML_MIME, "<h1>File not found.</h1>",
                             NULL);;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        snprintf(full_path, sizeof(full_path), "%s%s/index.html", www_folder,request->http_uri);
        if (stat(full_path, &file_stat) == -1) {

            return send_response(client_fd, NOT_FOUND, HTML_MIME, "<h1>File not found.</h1>",
                                 NULL);;
        }
    }
    fprintf(stderr, "%s\n", full_path);

    char last_modified[64];
    struct tm *gmt = gmtime(&file_stat.st_mtime);
    strftime(last_modified, sizeof(last_modified), "%a, %d %b %Y %H:%M:%S GMT",gmt);

    int file_fd = open(full_path, O_RDONLY);
    if (file_fd == -1) {
        return send_response(client_fd, BAD_REQUEST, "text/plain", "Unable to open file.",
                             NULL);;
    }

    size_t file_size = file_stat.st_size;
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "%ld", file_size);
    const char *mime_type = get_mime_type(full_path);

    if (request->http_method[0] == 'H') {

        return send_response(client_fd, OK, mime_type, NULL, last_modified);;
    }

    char *file_content = malloc(file_size);
    if (!file_content) {

        close(file_fd);
        return send_response(client_fd, BAD_REQUEST, "text/plain", "Memory allocation failed.", NULL);;
    }
    ssize_t bytes_read = read(file_fd, file_content, file_size);
    if (bytes_read != (ssize_t)file_size) {

        free(file_content);
        close(file_fd);
        return send_response(client_fd, BAD_REQUEST, "text/plain", "File read error.", NULL);;
    }
    close(file_fd);

    // Send headers
    char *resource;
    size_t resource_len;
    serialize_http_response(&resource, &resource_len, OK, (char *) mime_type,
                            content_length, last_modified, file_size, file_content);

    bool write_result = write_all(client_fd, resource, resource_len);

    free(resource);
    free(file_content);
    return write_result;
}

bool handle_post(Request *request, int client_fd,http_context* context,size_t content_length) {


    fprintf(stderr,"the post is size is %ld\n",request->status_header_size + content_length);
    fprintf(stderr,"The header is%ld\n",request->status_header_size);
    fprintf(stderr,"the content length is %ld\n",content_length);

    
    bool write_result = write_all(client_fd, context->request_buffer, request->status_header_size + content_length);
    fprintf(stderr,"the write result is%d\n",write_result);
    
    return write_result;


}

bool handle_request(Request *request, int client_fd, const char *www_folder,http_context* context,size_t content_length){


    char *close_state = get_header_value(request,CONNECTION_STR);

    bool close_result = false;

    if(close_state&&strcasecmp(close_state,CLOSE)==0){
        fprintf(stderr,"compare successful\n");
        close_result = true;
    }else{
        close_result = false;
    }
    bool is_write = true;

    if (strcmp(request->http_version, HTTP_VER) != 0){
        is_write = send_response(client_fd, BAD_REQUEST, "text/plain", "Wrong HTTP version", NULL);
        if(close_result){
            while(!is_write){
                is_write = send_response(client_fd, BAD_REQUEST, "text/plain", "Wrong HTTP version", NULL);

            }
        }


    }else{
        if(strcmp(request->http_method,GET)==0 || strcmp(request->http_method, HEAD) == 0){



            is_write=handle_get_head(request,client_fd,www_folder);

            if(close_result){
                while(!is_write){
                    is_write = handle_get_head(request,client_fd,www_folder);

                }
            }

        }else if(strcmp(request->http_method, POST) == 0){

            is_write=handle_post(request,client_fd,context,content_length);
            if(close_result){
                while(!is_write){
                    is_write = handle_post(request,client_fd,context,content_length);

                }
            }
        }else{
            is_write=send_response(client_fd, BAD_REQUEST, "text/plain", "Method wrong problem", NULL);
            //is_write = send_response(client_fd, BAD_REQUEST, "text/plain", "Wrong HTTP version", NULL);
            if(close_result){
                while(!is_write){
                    is_write = send_response(client_fd, BAD_REQUEST, "text/plain", "Method wrong problem", NULL);

                }
            }
        }

    }

    if(is_write){
        context->buffer_size-=request->status_header_size + content_length;

        if(context->buffer_size == 0){
            free(context->request_buffer);
            context->request_buffer = NULL;

        }else{

            char* new_request_buffer = malloc(context->buffer_size);
            memcpy(new_request_buffer,context->request_buffer+request->status_header_size + content_length,context->buffer_size);
            free(context->request_buffer);
            context->request_buffer = NULL;
            context->request_buffer = new_request_buffer;

        }

    }


    return close_result;
}

bool handle_client(int client_fd, const char *www_folder,http_context* context,int nfds) {

    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    ssize_t  total_read = 0;
    while(total_read < BUF_SIZE){

        ssize_t  bytes_read = read(client_fd, buffer + total_read, BUF_SIZE - total_read);

        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                fprintf(stderr,"The is read 0\n");
                return false;
            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // No more data available for now (non-blocking mode)
                    fprintf(stderr,"No data available, try again later.\n");
                    break;
                } else {
                    fprintf(stderr,"Error reading from file descriptor");

                    return true;
                }
            }
            // Indicate that the connection should be closed

        }
        total_read+=bytes_read;

    }

    fprintf(stderr,"The nfds is %d\n",nfds);

    char *new_buffer = realloc(context->request_buffer, context->buffer_size + total_read);
    if (!new_buffer){
        fprintf(stderr, "Failed to allocate memory for request buffer\n");
        return true; // Indicate that the connection should be closed
    }

    context->request_buffer = new_buffer;

    memcpy(context->request_buffer + context->buffer_size, buffer, total_read);

    context->buffer_size+=total_read;

    while(context->buffer_size > 0){

        Request request;
        memset(&request, 0, sizeof(request));

        test_error_code_t parse_result = parse_http_request(context->request_buffer,context->buffer_size,&request);

        if(parse_result == TEST_ERROR_NONE){

            char *content_length_str = get_header_value(&request, CONTENT_LENGTH_STR);
            size_t content_length = content_length_str ? strtoul(content_length_str, NULL, 10) : 0;

            //context->content_size <= context->body_size

            if(context->buffer_size >=request.status_header_size + content_length){


                bool is_close = handle_request(&request,client_fd,www_folder,context,content_length);

                if(is_close){


                    return true;
                }

                fprintf(stderr,"Correct here22\n");
                fprintf(stderr,"the close is %d\n",is_close);




            }else{
                // file content not fully get
                return false;
            }

        } else if (parse_result == TEST_ERROR_PARSE_PARTIAL){
            return false;
        }else if(parse_result == TEST_ERROR_PARSE_FAILED){
            fprintf(stderr,"parse error\n");
            return false;
        }else{
            return false;
        }
    }

    return false;

}


int main(int argc, char *argv[]) {
    /* Validate and parse args */
//    fprintf(stderr,"Correct here\n");
    if (argc != 2) {
        fprintf(stderr, "usage: %s <www-folder>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *www_folder = argv[1];

    DIR *www_dir = opendir(www_folder);
    if (www_dir == NULL) {
        fprintf(stderr, "Unable to open www folder %s.\n", www_folder);
        return EXIT_FAILURE;
    }
    closedir(www_dir);

    /* CP1: Set up sockets and read the buf */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        fprintf(stderr, "Socket creation failed\n");
        return EXIT_FAILURE;
    }
    make_socket_non_blocking(server_fd);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
               sizeof(opt));
    struct sockaddr_in server_addr = {.sin_family = AF_INET,
            .sin_port = htons(HTTP_PORT),
            .sin_addr.s_addr = INADDR_ANY};

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) ==-1) {
        fprintf(stderr, "Binding failed\n");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, MAX_CONNECTIONS) == -1) {
        fprintf(stderr, "Listen failed\n");
        close(server_fd);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "Server running on port %d, serving folder: %s\n", HTTP_PORT,
            www_folder);

    struct pollfd fds[MAX_CONNECTIONS + 1];

    fds[0].fd = server_fd;
    fds[0].events = POLLIN | POLLHUP;
    fds[0].revents = 0;
    int nfds = 1;

    http_context  request_storage[MAX_CONNECTIONS+1];


    while (true) {
        int poll_count = poll(fds, nfds, CONNECTION_TIMEOUT * 1000);
        if (poll_count == -1) {
            perror("Poll failed");
            break;
        }


        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                //fds[i].revents = 0; // Clear the event

                if (fds[i].fd == server_fd) {
                    // Accept all pending connections
                    while (true) {
                        int client_fd = accept(server_fd, NULL, NULL);
                        fprintf(stderr,"the client_fd is %d\n",client_fd);
                        if (client_fd == -1) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                break; // No more pending connections
                            } else {
                                fprintf(stderr,"accept failed");
                                break;
                            }
                        }

                        make_socket_non_blocking(client_fd);

                        if (nfds-1 >= MAX_CONNECTIONS) {
                            send_response(client_fd, SERVICE_UNAVAILABLE, HTML_MIME,
                                          "<p>Server is overloaded. Try again later.</p>", NULL);
                            close(client_fd);
                            continue;
                        }

                        fds[nfds].fd = client_fd;
                        fds[nfds].events = POLLIN | POLLHUP | POLLERR;
                        request_storage[nfds].request_buffer = NULL;
                        request_storage[nfds].buffer_size=0;

                        fprintf(stderr, "New client initialized at index %d\n", nfds);
                        nfds++;
                    }

                } else {
                    // Handle client request
                    fprintf(stderr,"the index is %d\n",i);
                    bool is_close = handle_client(fds[i].fd, www_folder, &request_storage[i],nfds);

                    fprintf(stderr,"the is close is %d\n",is_close);
                    if (is_close) {
                        fprintf(stderr,"The request storage buffer size %ld\n",request_storage[i].buffer_size);
                        fprintf(stderr, "Closing client at index %d\n", i);
                        close(fds[i].fd);
                        reset_context(&request_storage[i]);
                        fds[i] = fds[nfds - 1];
                        request_storage[i] = request_storage[nfds - 1];
                        nfds--;
                        i--;
                    }
                }
            } else if (fds[i].revents & POLLHUP || fds[i].revents & POLLERR) {
                fprintf(stderr, "Error or hangup on client at index %d\n", i);
                reset_context(&request_storage[i]);
                close(fds[i].fd);
                fds[i] = fds[nfds - 1];
                request_storage[i] = request_storage[nfds - 1];
                nfds--;
                i--;
            }
        }
    }

    close(server_fd);

    return EXIT_SUCCESS;
}
