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
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <poll.h>
#include <stdbool.h>

#include "parse_http.h"
#include "ports.h"

#define BUF_SIZE 8192
// Closes a client's connection if they have not sent a valid request within
// CONNECTION_TIMEOUT seconds.
#define CONNECTION_TIMEOUT 50
#define MAX_CONNECTIONS 100

const char *get_mime_type(const char *path) {
    fprintf(stderr,"Get mime type\n");
    const char *ext = strrchr(path, '.'); // Get file extension
    if (!ext || ext == path) return OCTET_MIME; // Default for unknown or missing types

    ext++; // Move past the dot to get the extension without "."

    if (strcmp(ext, HTML_EXT) == 0) return HTML_MIME;
    if (strcmp(ext, CSS_EXT) == 0) return CSS_MIME;
    if (strcmp(ext, PNG_EXT) == 0) return PNG_MIME;
    if (strcmp(ext, JPG_EXT) == 0) return JPG_MIME;
    if (strcmp(ext, GIF_EXT) == 0) return GIF_MIME;

    return OCTET_MIME; // Default fallback
}

void send_response(int client_fd, const char *status, const char *content_type, const char *body, const char *last_modified){

    char *response;
    size_t response_len;
    char content_length [32];
    snprintf(content_length, sizeof(content_length), "%ld", body ? strlen(body) : 0);
    serialize_http_response(&response, &response_len, status, (char *)content_type, content_length,
                            (char *)last_modified, body ? strlen(body) : 0, (char *)body);

    write(client_fd,response,response_len);
    free(response);
    close(client_fd);

}


void handle_client(int client_fd, const char *www_folder){

    char buffer[HTTP_SIZE];
    memset(buffer,0,HTTP_SIZE);

    ssize_t bytes_read = read(client_fd,buffer,HTTP_SIZE-1);
    if(bytes_read <=0){
        close(client_fd);
        fprintf(stderr,"bytes_read failed\n");
        return;
    }

    Request request;
    memset(&request,0,sizeof(Request));

    test_error_code_t parse_result = parse_http_request(buffer, bytes_read, &request);

    switch (parse_result) {
        case TEST_ERROR_NONE:
            break;
        case TEST_ERROR_PARSE_PARTIAL:
            send_response(client_fd, BAD_REQUEST, "text/plain", "Request incomplete", NULL);
            return;
        case TEST_ERROR_PARSE_FAILED:
            send_response(client_fd, BAD_REQUEST, "text/plain", "Malformed request", NULL);
            return;

        case TEST_ERROR_HTTP_CONNECT_FAILED:
            send_response(client_fd, BAD_REQUEST, "text/plain", "Malformed request", NULL);
            return;
        case TEST_ERROR_HTTP_SEND_FAILED:
            send_response(client_fd, BAD_REQUEST, "text/plain", "Malformed request", NULL);
            return;
        default:
            send_response(client_fd, BAD_REQUEST, "text/plain", "Malformed request", NULL);
            return;

    }

    char full_path[4096];
    memset(full_path,0,HTTP_SIZE);
    snprintf(full_path, sizeof(full_path), "%s%s", www_folder, request.http_uri);

    fprintf(stderr,"%s\n",full_path);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        send_response(client_fd, NOT_FOUND, HTML_MIME, "<h1>File not found.</h1>", NULL);
        return;
    }

    char last_modified[64];
    struct tm *gmt = gmtime(&file_stat.st_mtime);
    strftime(last_modified, sizeof(last_modified), "%a, %d %b %Y %H:%M:%S GMT", gmt);



    switch (request.http_method[0]) {
        case 'G':
            if (strcmp(request.http_method, GET) == 0) {
                int file_fd = open(full_path, O_RDONLY);
                if (file_fd == -1) {
                    send_response(client_fd, BAD_REQUEST, "text/plain", "Unable to open file.", NULL);
                    return;
                }

                size_t file_size = file_stat.st_size;
                char content_length[32];
                snprintf(content_length, sizeof(content_length), "%ld", file_size);
                const char *mime_type = get_mime_type(full_path);

                // Send headers
                char *headers;
                size_t headers_len;
                serialize_http_response(&headers, &headers_len, OK, (char *)mime_type, content_length, last_modified, 0, NULL);
                if (write(client_fd, headers, headers_len) != headers_len) {
                    fprintf(stderr, "Error writing headers to client\n");
                    free(headers);
                    close(file_fd);
                    close(client_fd);
                    return;
                }
                free(headers);

                // Send file content in chunks
                char file_buffer[HTTP_SIZE];
                memset(file_buffer,0,HTTP_SIZE);
                ssize_t file_bytes_read;
                while ((file_bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
                    if (write(client_fd, file_buffer, file_bytes_read) != file_bytes_read) {
                        fprintf(stderr, "Error writing file content to client\n");
                        break;
                    }
                }

                if (file_bytes_read < 0) {
                    fprintf(stderr, "Error reading file\n");
                }

                close(file_fd);
                close(client_fd);
            }
            break;

        case 'H':
            fprintf(stderr,"Call header\n");
            if (strcmp(request.http_method, HEAD) == 0) {

                // Get MIME type
                const char *mime_type = get_mime_type(full_path);

                // Send response without file content (HEAD only needs headers)
                send_response(client_fd, OK, mime_type, NULL, last_modified);
            }
            break;
        case 'P': // POST
            if (strcmp(request.http_method, POST) == 0) {
                send_response(client_fd, OK, "text/plain", request.body ? request.body : "", NULL);
            }
            break;

        default: // Unsupported HTTP methods
            send_response(client_fd, BAD_REQUEST, HTML_MIME, "<p>Unsupported HTTP method.</p>", NULL);
            break;


    }







}


int main(int argc, char *argv[]) {
    /* Validate and parse args */
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
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd== -1){
        fprintf(stderr,"Socket creation failed\n");
        return EXIT_FAILURE;

    }

    int opt = 1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));
    struct sockaddr_in server_addr = {.sin_family = AF_INET,.sin_port = htons(HTTP_PORT),.sin_addr.s_addr = INADDR_ANY};

    if(bind(server_fd,(struct  sockaddr*)&server_addr,sizeof(server_addr))==-1){
        fprintf(stderr,"Binding failed\n");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if(listen(server_fd,MAX_CONNECTIONS)==-1){
        fprintf(stderr,"Listen failed\n");
        close(server_fd);
        return EXIT_FAILURE;
    }
    fprintf(stderr,"Server running on port %d, serving folder: %s\n", HTTP_PORT, www_folder);

    struct pollfd fds[MAX_CONNECTIONS+1];

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    int nfds = 1;

    while(true){
        int poll_count = poll(fds,nfds,CONNECTION_TIMEOUT*1000);
        if(poll_count == -1){
            fprintf(stderr,"Poll failed\n");
            break;
        }
        for(int i = 0;i < nfds;i++){

            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    // Accept new client connection
                    int client_fd = accept(server_fd, NULL, NULL);
                    if (client_fd == -1) {
                        fprintf(stderr,"Accept failed\n");
                        continue;
                    }

                    if (nfds - 1 >= MAX_CONNECTIONS) {
                        send_response(client_fd, SERVICE_UNAVAILABLE, HTML_MIME,
                                      "<p>Server is overloaded. Try again later.</p>", NULL);
                        continue; // client_fd is closed in send_response
                    }

                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } else {
                    // Handle client request
                    handle_client(fds[i].fd, www_folder);

                    // Remove client from poll array
                    fds[i] = fds[nfds - 1];
                    nfds--;
                }
            }




        }

    }

    close(server_fd);


    return EXIT_SUCCESS;
}
