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

void send_response(int client_fd, const char *status, const char *content_type,
                   const char *body, const char *last_modified) {

    char *response;
    size_t response_len;
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "%ld",
             body ? strlen(body) : 0);
    serialize_http_response(
            &response, &response_len, status, (char *) content_type, content_length,
            (char *) last_modified, body ? strlen(body) : 0, (char *) body);

    write(client_fd, response, response_len);
    free(response);
}

void handle_get_header(Request *request, int client_fd, const char *www_folder) {
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
        send_response(client_fd, NOT_FOUND, HTML_MIME, "<h1>File not found.</h1>",
                      NULL);
        return;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        snprintf(full_path, sizeof(full_path), "%s%s/index.html", www_folder,request->http_uri);
        if (stat(full_path, &file_stat) == -1) {
            send_response(client_fd, NOT_FOUND, HTML_MIME, "<h1>File not found.</h1>",
                          NULL);
            return;
        }
    }
    fprintf(stderr, "%s\n", full_path);

    char last_modified[64];
    struct tm *gmt = gmtime(&file_stat.st_mtime);
    strftime(last_modified, sizeof(last_modified), "%a, %d %b %Y %H:%M:%S GMT",
             gmt);

    int file_fd = open(full_path, O_RDONLY);
    if (file_fd == -1) {
        send_response(client_fd, BAD_REQUEST, "text/plain", "Unable to open file.",
                      NULL);
        return;
    }

    size_t file_size = file_stat.st_size;
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "%ld", file_size);
    const char *mime_type = get_mime_type(full_path);

    if (request->http_method[0] == 'H') {
        send_response(client_fd, OK, mime_type, NULL, last_modified);
        return;
    }

    // Send headers
    char *headers;
    size_t headers_len;
    serialize_http_response(&headers, &headers_len, OK, (char *) mime_type,
                            content_length, last_modified, 0, NULL);
    if (write(client_fd, headers, headers_len) != headers_len) {
        fprintf(stderr, "Error writing headers to client\n");
        free(headers);
        close(file_fd);
        return;
    }
    free(headers);

    // Send file content in chunks
    char file_buffer[BUF_SIZE];
    memset(file_buffer, 0, BUF_SIZE);
    ssize_t file_bytes_read;
    while ((file_bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) >
           0) {
        if (write(client_fd, file_buffer, file_bytes_read) != file_bytes_read) {
            fprintf(stderr, "Error writing file content to client\n");
            break;
        }
    }

    if (file_bytes_read < 0) {
        fprintf(stderr, "Error reading file\n");
    }
    close(file_fd);
}

void handle_post(Request *request, int client_fd, const char *www_folder) {
    // implement the handle post just echo the request body, also implement the
    // large post request in the client Extract the Content-Length header to
    // determine the body size Extract the body from the request
    char *body = request->body;
    size_t body_len = strlen(body);

    // Prepare the response headers
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "%ld", body_len);

    // Send the response
    char *response;
    size_t response_len;
    serialize_http_response(&response, &response_len, OK, "text/plain",
                            content_length, NULL, body_len, body);

    if (write(client_fd, response, response_len) != response_len) {
        fprintf(stderr, "Error writing response to client\n");
    }

    free(response);
}

void handle_client(int client_fd, const char *www_folder) {
    char buffer[BUF_SIZE];
    ssize_t bytes_read;

    memset(buffer, 0, BUF_SIZE);

    bytes_read = read(client_fd, buffer, BUF_SIZE - 1);
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            // Client closed the connection
            fprintf(stderr, "Client closed the connection\n");
        } else {
            fprintf(stderr, "bytes_read failed\n");
        }
        close(client_fd);
        return;
    }
    buffer[bytes_read] = '\0';

    Request request;
    memset(&request, 0, sizeof(Request));

    test_error_code_t parse_result = parse_http_request(buffer, bytes_read, &request);

    if (parse_result == TEST_ERROR_NONE) {
        bool close_connection = false;
        for (int i = 0; i < request.header_count; i++) {
            fprintf(stderr, "Header: %s: %s \n", request.headers[i].header_name,
                    request.headers[i].header_value);
            if (strcasecmp(request.headers[i].header_name, "Connection") == 0 &&
                strcasecmp(request.headers[i].header_value, "close") == 0) {
                close_connection = true;
                break;
            }
        }
        if(strcmp(request.http_version,HTTP_VER)!=0) {
            send_response(client_fd, BAD_REQUEST, "text/plain",
                          "Wrong HTTP version", NULL);
            if (close_connection) {
                fprintf(stderr, "Closing connection based on the Connection close\n");
                close(client_fd);
                return;
            }

        }


            if (strcmp(request.http_method, GET) == 0 ||strcmp(request.http_method, HEAD) == 0) {

                handle_get_header(&request, client_fd, www_folder);
            } else if (strcmp(request.http_method, POST) == 0) {
                handle_post(&request, client_fd, www_folder);
            } else {
                send_response(client_fd, BAD_REQUEST, "text/plain",
                              "Method wrong problem", NULL);
                //close(client_fd);
            }


        // Check if the connection should be closed

        if (close_connection) {
            fprintf(stderr, "Closing connection based on the Connection close\n");
            close(client_fd);
            return;
        }

    } else {
        send_response(client_fd, BAD_REQUEST, "text/plain", "Malformed request",
                      NULL);
        close(client_fd);
        return;
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
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        fprintf(stderr, "Socket creation failed\n");
        return EXIT_FAILURE;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
               sizeof(opt));
    struct sockaddr_in server_addr = {.sin_family = AF_INET,
            .sin_port = htons(HTTP_PORT),
            .sin_addr.s_addr = INADDR_ANY};

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) ==
        -1) {
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


    while (true) {
        int poll_count = poll(fds, nfds, CONNECTION_TIMEOUT);
        if (poll_count == -1) {
            fprintf(stderr, "Poll failed\n");
            break;
        }

        for (int i = 0; i < nfds; i++) {

            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    // Accept new client connection
                    int client_fd = accept(server_fd, NULL, NULL);
                    if (client_fd == -1) {
                        fprintf(stderr, "Accept failed\n");
                        continue;
                    }

                    if (nfds - 1 >= MAX_CONNECTIONS) {
                        send_response(client_fd, SERVICE_UNAVAILABLE, HTML_MIME,
                                      "<p>Server is overloaded. Try again later.</p>",
                                      NULL);
                        close(client_fd);
                        continue; // client_fd is closed in send_response
                    }

                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN | POLLHUP | POLLERR;
                    nfds++;
                } else {
                    // Handle client request
                    handle_client(fds[i].fd, www_folder);

                    // Remove client from poll array
                }
            } else if (fds[i].revents & POLLHUP || fds[i].revents & POLLERR ) {
                fprintf(stderr,"poll handle close\n");
                fds[i] = fds[nfds - 1];
                nfds--;
                i--;

            }
        }
    }

    close(server_fd);

    return EXIT_SUCCESS;
}
