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

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h>
#include <poll.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

#include "parse_http.h"
#include "test_error.h"
#include "ports.h"

#define BUF_SIZE 8192
#define MAX_RESOURCES 100
#define WWW_FOLDER "./www"
#define MAX_RETRIES 3
#define POLL_TIMEOUT 5000
#define RECONNECT_TIMEOUT 100

// Define default values for parallel connections and inflight requests
#ifndef PARALLEL_CONNECTIONS
#define PARALLEL_CONNECTIONS 4  
#endif

#ifndef MAX_INFLIGHT_REQUESTS
#define MAX_INFLIGHT_REQUESTS 6  
#endif

typedef enum {
    RESOURCE_PENDING,
    RESOURCE_NOT_FOUND,
    RESOURCE_ERROR,
    RESOURCE_DOWNLOADED
} ResourceState;

typedef enum {
    CONNECTION_STATE_DISCONNECTED,
    CONNECTION_STATE_CONNECTING,
    CONNECTION_STATE_CONNECTED,
    CONNECTION_STATE_FAILED
} ConnectionState;

typedef struct {
    char uri[HTTP_SIZE];
    char dependency[HTTP_SIZE];
    ResourceState state;
    bool requested;
    int retry_count;
    int assigned_connection;
} Resource;
typedef struct WriteBuffer {
    char *buffer;
    size_t size;
    size_t offset;
    struct WriteBuffer *next;
} WriteBuffer;
typedef struct {
    char *buffer;
    size_t buffer_size;
    size_t content_length;
    bool headers_parsed;
    char current_uri[HTTP_SIZE];
    int status_code;
    bool write_pending;
    char *write_buffer;
    size_t write_size;
    size_t write_offset;
} ResponseState;

typedef struct {
    int sock_fd;
    ResponseState response;
    char inflight_requests[MAX_INFLIGHT_REQUESTS][HTTP_SIZE];
    int num_inflight;
    ConnectionState state;
    size_t total_requests;
     int index;
    time_t last_activity;
    bool processing_response;
    WriteBuffer *write_queue_head;  // Pointer to the head of the write buffer queue
    WriteBuffer *write_queue_tail;  // Pointer to the tail of the write buffer queue
} Connection;


// Global variables
static Resource resources[MAX_RESOURCES];
static int resource_count = 0;
static Connection *connections = NULL;
static int num_connections = 0;

void init_response_state(ResponseState *state) {
    if (state->buffer) {
        free(state->buffer);
    }
    state->buffer = NULL;
    state->buffer_size = 0;
    state->content_length = 0;
    state->headers_parsed = false;
    state->status_code = 0;
    state->write_pending = false;
    state->write_buffer = NULL;
    state->write_size = 0;
    state->write_offset = 0;
    memset(state->current_uri, 0, sizeof(state->current_uri));
}

void make_socket_non_blocking(int sock_fd) {
    int flags = fcntl(sock_fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    if (fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL O_NONBLOCK");
        exit(EXIT_FAILURE);
    }
}

void close_connection(Connection *conn) {
    if (conn->sock_fd != -1) {
        close(conn->sock_fd);
        conn->sock_fd = -1;
    }

    // Mark inflight resources as not requested and handle retries
    for (int i = 0; i < conn->num_inflight; i++) {
        char *uri = conn->inflight_requests[i];
        for (int j = 0; j < resource_count; j++) {
            if (strcmp(resources[j].uri, uri) == 0) {
                resources[j].retry_count++;
                if (resources[j].retry_count >= MAX_RETRIES) {
                    resources[j].state = RESOURCE_ERROR;
                    fprintf(stderr, "Resource '%s' reached max retries. Marked as ERROR.\n", resources[j].uri);
                } else {
                    resources[j].requested = false;
                    resources[j].assigned_connection = -1;
                    fprintf(stderr, "Resource '%s' will be retried. Retry count: %d\n", resources[j].uri, resources[j].retry_count);
                }
                break;
            }
        }
    }
    conn->num_inflight = 0;

    init_response_state(&conn->response);
    conn->state = CONNECTION_STATE_DISCONNECTED;
    conn->processing_response = false;

    // Free any pending write buffers
    WriteBuffer *current = conn->write_queue_head;
    while (current) {
        WriteBuffer *next = current->next;
        free(current->buffer);
        free(current);
        current = next;
    }
    conn->write_queue_head = conn->write_queue_tail = NULL;
}


test_error_code_t setup_connection(Connection *conn, struct sockaddr_in server_addr) {
    if (conn->sock_fd != -1) {
        close(conn->sock_fd);
    }

    conn->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (conn->sock_fd < 0) {
        return TEST_ERROR_HTTP_CONNECT_FAILED;
    }

    make_socket_non_blocking(conn->sock_fd);

    if (connect(conn->sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        if (errno != EINPROGRESS) {
            close(conn->sock_fd);
            conn->sock_fd = -1;
            return TEST_ERROR_HTTP_CONNECT_FAILED;
        }
    }

    init_response_state(&conn->response);
    conn->num_inflight = 0;
    conn->state = CONNECTION_STATE_CONNECTING;
    conn->last_activity = time(NULL);
    return TEST_ERROR_NONE;
}

void *portable_memmem(const void *haystack, size_t haystack_len, const void *needle, size_t needle_len) {
    if (!haystack || !needle || needle_len == 0 || haystack_len < needle_len) {
        return NULL;
    }

    const char *h = haystack;
    const char *n = needle;

    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        if (memcmp(h + i, n, needle_len) == 0) {
            return (void *)(h + i);
        }
    }

    return NULL;
}


bool parse_dependency_csv(const char *content, size_t content_length) {
    char *content_copy = strndup(content, content_length);
    if (!content_copy) return false;

    char *saveptr1 = NULL;
    char *line = strtok_r(content_copy, "\n", &saveptr1);

    while (line && resource_count < MAX_RESOURCES) {
        char *saveptr2 = NULL;
        char *uri = strtok_r(line, ",", &saveptr2);
        char *dependency = strtok_r(NULL, ",", &saveptr2);

        if (uri) {
            // Trim whitespaces from uri
            trim_whitespace(uri, strlen(uri));

            // Add leading slash to URI if missing
            char full_uri[HTTP_SIZE];
            if (uri[0] != '/') {
                snprintf(full_uri, sizeof(full_uri), "/%s", uri);
            } else {
                strncpy(full_uri, uri, sizeof(full_uri) - 1);
                full_uri[sizeof(full_uri) - 1] = '\0';
            }

            strncpy(resources[resource_count].uri, full_uri, HTTP_SIZE - 1);
            resources[resource_count].uri[HTTP_SIZE - 1] = '\0';
            resources[resource_count].state = RESOURCE_PENDING;
            resources[resource_count].requested = false;
            resources[resource_count].retry_count = 0;
            resources[resource_count].dependency[0] = '\0';

            // Handle dependencies
            if (dependency) {
                // Trim whitespaces from dependency
                trim_whitespace(dependency, strlen(dependency));
                if (*dependency) {
                    char full_dependency[HTTP_SIZE];
                    if (dependency[0] != '/') {
                        snprintf(full_dependency, sizeof(full_dependency), "/%s", dependency);
                    } else {
                        strncpy(full_dependency, dependency, sizeof(full_dependency) - 1);
                        full_dependency[sizeof(full_dependency) - 1] = '\0';
                    }
                    strncpy(resources[resource_count].dependency, full_dependency, HTTP_SIZE - 1);
                    resources[resource_count].dependency[HTTP_SIZE - 1] = '\0';
                }
            }

            fprintf(stderr, "Added resource: URI=%s, Dependency=%s\n",
                    resources[resource_count].uri,
                    resources[resource_count].dependency[0] ? resources[resource_count].dependency : "none");

            resource_count++;
        }
        line = strtok_r(NULL, "\n", &saveptr1);
    }

    free(content_copy);
    return true;
}

Connection* find_best_connection() {
    for (int i = 0; i < num_connections; i++) {
        if (connections[i].state == CONNECTION_STATE_CONNECTED &&
            connections[i].num_inflight == 0) {
            return &connections[i];
        }
    }
    return NULL;
}

test_error_code_t queue_http_request(Connection *conn, Resource *resource,int conn_index) {
    Request request;
    memset(&request, 0, sizeof(Request));

    // Set HTTP method, URI, and version
    strncpy(request.http_method, GET, sizeof(request.http_method) - 1);

    // Ensure the URI has a leading slash
    char full_uri[HTTP_SIZE];
    if (resource->uri[0] != '/') {
        snprintf(full_uri, sizeof(full_uri), "/%s", resource->uri);
    } else {
        strncpy(full_uri, resource->uri, sizeof(full_uri) - 1);
        full_uri[sizeof(full_uri) - 1] = '\0';
    }
    strncpy(request.http_uri, full_uri, sizeof(request.http_uri) - 1);
    request.http_uri[sizeof(request.http_uri) - 1] = '\0';

    strncpy(request.http_version, HTTP_VER, sizeof(request.http_version) - 1);

    // Allocate and set headers
    request.header_count = 2;
    request.allocated_headers = 2;
    request.headers = malloc(request.header_count * sizeof(Request_header));
    if (!request.headers) {
        return TEST_ERROR_HTTP_SEND_FAILED;
    }

    // Set "Connection: keep-alive"
    strncpy(request.headers[0].header_name, CONNECTION_STR, sizeof(request.headers[0].header_name) - 1);
    strncpy(request.headers[0].header_value, CONNECTION_VAL, sizeof(request.headers[0].header_value) - 1);

    // Set "Host: <server-ip>:<port>"
    strncpy(request.headers[1].header_name, HOST, sizeof(request.headers[1].header_name) - 1);
    strncpy(request.headers[1].header_value, "127.0.0.1:20080", sizeof(request.headers[1].header_value) - 1);

    ResponseState *state = &conn->response;

    // Allocate buffer for the serialized request
    char *request_buffer = malloc(HTTP_SIZE);
    if (!request_buffer) {
        free(request.headers);
        return TEST_ERROR_HTTP_SEND_FAILED;
    }
    size_t request_size = 0;

    // Serialize the request
    test_error_code_t result = serialize_http_request(request_buffer, &request_size, &request);
    free(request.headers);
    if (result != TEST_ERROR_NONE) {
        free(request_buffer);
        return result;
    }
  WriteBuffer *new_write = malloc(sizeof(WriteBuffer));
    if (!new_write) {
        free(request_buffer);
        return TEST_ERROR_HTTP_SEND_FAILED;
    }
    new_write->buffer = request_buffer;
    new_write->size = request_size;
    new_write->offset = 0;
    new_write->next = NULL;

    if (conn->write_queue_tail) {
        conn->write_queue_tail->next = new_write;
        conn->write_queue_tail = new_write;
    } else {
        conn->write_queue_head = conn->write_queue_tail = new_write;
    }


    // Update inflight requests
    strncpy(conn->inflight_requests[conn->num_inflight], resource->uri, HTTP_SIZE - 1);
    conn->inflight_requests[conn->num_inflight][HTTP_SIZE - 1] = '\0';
    conn->num_inflight++;

    // Set resource as requested
    resource->requested = true;
    resource->assigned_connection = resource->assigned_connection = conn_index;

    fprintf(stderr, "Queued request for: %s on connection %ld\n", resource->uri, conn_index);
    return TEST_ERROR_NONE;
}

test_error_code_t queue_pending_requests() {
    fprintf(stderr, "Checking for pending requests...\n");

    for (int i = 0; i < resource_count; i++) {
        if (!resources[i].requested && resources[i].state == RESOURCE_PENDING) {
           if( resources[i].retry_count < MAX_RETRIES) {
            bool can_request = true;

            // Check dependency
            if (resources[i].dependency[0] != '\0') {
                can_request = false;
                for (int j = 0; j < resource_count; j++) {
                    if (strcmp(resources[j].uri, resources[i].dependency) == 0) {
                        if (resources[j].state == RESOURCE_DOWNLOADED) {
                            can_request = true;
                            fprintf(stderr, "Dependency '%s' for '%s' is satisfied\n",
                                    resources[i].dependency, resources[i].uri);
                            break;
                        } else {
                            fprintf(stderr, "Dependency '%s' for '%s' is not yet downloaded\n",
                                    resources[i].dependency, resources[i].uri);
                        }
                    }
                }
            }

            if (can_request) {
                // Try all available connections
                for (int j = 0; j < num_connections; j++) {
                    if (connections[j].state == CONNECTION_STATE_CONNECTED &&
                        connections[j].num_inflight < MAX_INFLIGHT_REQUESTS) {

                        if (queue_http_request(&connections[j], &resources[i],j) == TEST_ERROR_NONE) {
                            fprintf(stderr, "Successfully queued '%s' on connection %d\n",
                                    resources[i].uri, j);
                            break;
                        }
                    }
                }
            }
        }
        }
    }

    // Print current state
    fprintf(stderr, "\nCurrent resource states:\n");
    for (int i = 0; i < resource_count; i++) {
        fprintf(stderr, "Resource '%s': %s (requested=%d, conn=%d)\n",
                resources[i].uri,
                resources[i].state == RESOURCE_DOWNLOADED ? "DOWNLOADED" :
                resources[i].state == RESOURCE_PENDING ? "PENDING" : "OTHER",
                resources[i].requested,
                resources[i].assigned_connection);
    }

    return TEST_ERROR_NONE;
}

test_error_code_t try_send_request(Connection *conn) {
    while (conn->write_queue_head) {
        WriteBuffer *current_write = conn->write_queue_head;
        while (current_write->offset < current_write->size) {
            ssize_t sent = write(conn->sock_fd,
                                 current_write->buffer + current_write->offset,
                                 current_write->size - current_write->offset);

            if (sent < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    return TEST_ERROR_PARSE_PARTIAL;
                }
                return TEST_ERROR_HTTP_SEND_FAILED;
            }
            current_write->offset += sent;
        }
        // Finished sending this write buffer
        conn->write_queue_head = current_write->next;
        if (conn->write_queue_head == NULL) {
            conn->write_queue_tail = NULL;
        }
        free(current_write->buffer);
        free(current_write);
    }
    return TEST_ERROR_NONE;
}


void save_file(const char *uri, const char *body, size_t body_length) {
    char filepath[PATH_MAX];

    // Remove leading "/" from URI if present
    if (uri[0] == '/') {
        uri++;
    }

    // Construct the file path
    snprintf(filepath, sizeof(filepath), "%s/%s", WWW_FOLDER, uri);

    // Create directory if needed
    char *dir = strdup(filepath);
    if (dir) {
        char *dirpath = dirname(dir);
        if (mkdir(dirpath, S_IRWXU) == -1 && errno != EEXIST) {
            fprintf(stderr, "Failed to create directory %s: %s\n", dirpath, strerror(errno));
        }
        free(dir);
    }

    // Open the file for writing
    FILE *fp = fopen(filepath, "wb");
    if (fp) {
        // Write the body to the file
        fwrite(body, 1, body_length, fp);
        fclose(fp);
        fprintf(stderr, "Saved file to: %s\n", filepath);
    } else {
        fprintf(stderr, "Failed to open file for writing: %s (error: %s)\n", filepath, strerror(errno));
    }
}

test_error_code_t process_response(Connection *conn) {
    ResponseState *state = &conn->response;
    size_t buffer_offset = 0;

    // Loop to process all complete responses in the buffer
    while (buffer_offset < state->buffer_size) {
        if (!state->headers_parsed) {
            // Look for the end of the headers
            char *headers_end = portable_memmem(state->buffer + buffer_offset, state->buffer_size - buffer_offset, "\r\n\r\n", 4);
            if (!headers_end) {
                // Incomplete headers; stop processing
                break;
            }

            size_t headers_length = headers_end - (state->buffer + buffer_offset) + 4;

            // Parse the status line
            char status_line[HTTP_SIZE];
            char *status_end = memchr(state->buffer + buffer_offset, '\n', state->buffer_size - buffer_offset);
            if (!status_end || status_end > headers_end) {
                // Incomplete status line; stop processing
                break;
            }

            size_t status_len = status_end - (state->buffer + buffer_offset);
            strncpy(status_line, state->buffer + buffer_offset, status_len);
            status_line[status_len] = '\0';

            if (sscanf(status_line, "HTTP/1.%*d %d", &state->status_code) != 1) {
                return TEST_ERROR_PARSE_FAILED;
            }

            // Extract Content-Length
            state->content_length = 0;
            char *content_length_str = portable_memmem(state->buffer + buffer_offset, headers_length, "Content-Length: ", 16);
            if (content_length_str) {
                content_length_str += 16;
                state->content_length = strtoul(content_length_str, NULL, 10);
            }

            // Mark headers as parsed
            state->headers_parsed = true;
            buffer_offset += headers_length;
        }

        if (state->headers_parsed) {
            size_t total_response_size = buffer_offset + state->content_length;
            if (state->buffer_size >= total_response_size) {
                // A complete response is available
                char *body = state->buffer + buffer_offset;
                size_t body_length = state->content_length;

                // Process the response body
                if (conn->num_inflight > 0) {
                    char *current_uri = conn->inflight_requests[0];

                    if (state->status_code == 200) {
                        if (strcmp(current_uri, "/dependency.csv") == 0) {
                            // Parse dependency.csv
                            if (!parse_dependency_csv(body, body_length)) {
                                return TEST_ERROR_PARSE_FAILED;
                            }

                            // Mark `/dependency.csv` as downloaded
                            for (int i = 0; i < resource_count; i++) {
                                if (strcmp(resources[i].uri, current_uri) == 0) {
                                    resources[i].state = RESOURCE_DOWNLOADED;
                                    resources[i].requested = true;
                                    fprintf(stderr, "Marked resource %s as downloaded\n", current_uri);
                                    break;
                                }
                            }
                        } else {
                            // Save the file
                            save_file(current_uri, body, body_length);

                            // Mark the resource as downloaded
                            for (int i = 0; i < resource_count; i++) {
                                if (strcmp(resources[i].uri, current_uri) == 0) {
                                    resources[i].state = RESOURCE_DOWNLOADED;
                                    fprintf(stderr, "Marked resource %s as downloaded\n", current_uri);
                                    break;
                                }
                            }
                        }
                    } else {
                        // Handle non-200 responses
                        fprintf(stderr, "Received non-200 response for %s: %d\n", current_uri, state->status_code);
                    }

                    // Shift inflight_requests
                    for (int i = 0; i < conn->num_inflight - 1; i++) {
                        strncpy(conn->inflight_requests[i], conn->inflight_requests[i + 1], HTTP_SIZE);
                    }
                    conn->num_inflight--;
                } else {
                    fprintf(stderr, "No inflight requests to match response\n");
                    return TEST_ERROR_PARSE_FAILED;
                }

                // Reset state for the next response
                state->headers_parsed = false;
                state->status_code = 0;
                state->content_length = 0;

                buffer_offset += body_length;
            } else {
                // Incomplete body; stop processing
                break;
            }
        }
    }

    // Remove processed data from the buffer
    if (buffer_offset > 0) {
        memmove(state->buffer, state->buffer + buffer_offset, state->buffer_size - buffer_offset);
        state->buffer_size -= buffer_offset;
    }

    return TEST_ERROR_NONE;
}

void monitor_connections() {
    time_t current_time = time(NULL);

    for (int i = 0; i < num_connections; i++) {
        Connection *conn = &connections[i];

        if (conn->state == CONNECTION_STATE_CONNECTED &&
            current_time - conn->last_activity > POLL_TIMEOUT / 1000) {

            fprintf(stderr, "Connection %d timed out, resetting\n", i);
            close_connection(conn);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <server-ip>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Create WWW folder
    mkdir(WWW_FOLDER, S_IRWXU);

    // Initialize multiple connections
    num_connections = PARALLEL_CONNECTIONS;  // Use PARALLEL_CONNECTIONS
    connections = calloc(num_connections, sizeof(Connection));
    struct pollfd *pfds = calloc(num_connections, sizeof(struct pollfd));

    // Set up server address
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(HTTP_PORT)
    };

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address\n");
        return EXIT_FAILURE;
    }

    // Initialize all connections
    for (int i = 0; i < num_connections; i++) {
        if (setup_connection(&connections[i], server_addr) != TEST_ERROR_NONE) {
            fprintf(stderr, "Failed to setup connection %d\n", i);
            continue;
        }
        pfds[i].fd = connections[i].sock_fd;
        pfds[i].events = POLLIN | POLLOUT;
    }

    // Initialize resources[] with /dependency.csv
    resource_count = 0;
    strncpy(resources[resource_count].uri, "/dependency.csv", HTTP_SIZE - 1);
    resources[resource_count].uri[HTTP_SIZE - 1] = '\0';
    resources[resource_count].dependency[0] = '\0'; // No dependency
    resources[resource_count].state = RESOURCE_PENDING;
    resources[resource_count].requested = false;
    resources[resource_count].retry_count = 0;
    resource_count++;

    // Find /dependency.csv in resources[]
    Resource *dependency_csv_resource = NULL;
    for (int i = 0; i < resource_count; i++) {
        if (strcmp(resources[i].uri, "/dependency.csv") == 0) {
            dependency_csv_resource = &resources[i];
            break;
        }
    }
    if (dependency_csv_resource == NULL) {
        fprintf(stderr, "Failed to find /dependency.csv in resources[]\n");
        return EXIT_FAILURE;
    }
    if (queue_http_request(&connections[0], dependency_csv_resource,connections[0].index) != TEST_ERROR_NONE) {
        fprintf(stderr, "Failed to queue initial request\n");
        return EXIT_FAILURE;
    }
    connections[0].total_requests++;

    bool all_done = false;
    while (!all_done) {
        monitor_connections();

        int poll_result = poll(pfds, num_connections, POLL_TIMEOUT);
        if (poll_result < 0) {
            perror("poll failed");
            break;
        }
    // Try to reestablish failed connections
    for (int i = 0; i < num_connections; i++) {
        if (pfds[i].fd == -1) {
            test_error_code_t result = setup_connection(&connections[i], server_addr);
            if (result == TEST_ERROR_NONE) {
                pfds[i].fd = connections[i].sock_fd;
                pfds[i].events = POLLIN | POLLOUT;
                fprintf(stderr, "Reestablished connection %d\n", i);
            }
        }
    }
      queue_pending_requests();
        // Handle all connections
        for (int i = 0; i < num_connections; i++) {
            if (pfds[i].fd == -1) continue;
            if (pfds[i].revents & POLLOUT) {
                // Handle connection establishment
                if (connections[i].state == CONNECTION_STATE_CONNECTING) {
                    int error = 0;
                    socklen_t len = sizeof(error);
                    if (getsockopt(pfds[i].fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0) {
                        connections[i].state = CONNECTION_STATE_CONNECTED;
                        fprintf(stderr, "Connection %d established\n", i);
                    } else {
                        fprintf(stderr, "Connection %d failed to establish: %s\n", i, strerror(error));
                        close_connection(&connections[i]);
                        pfds[i].fd = -1;
                        continue;
                    }
                }
                // Try to send pending requests
                if (connections[i].write_queue_head != NULL) {
                    test_error_code_t result = try_send_request(&connections[i]);
                    if (result == TEST_ERROR_HTTP_SEND_FAILED) {
                        fprintf(stderr, "Failed to send request on connection %d\n", i);
                        close_connection(&connections[i]);
                        pfds[i].fd = -1;
                        continue;
                    }
                }
            }
            if (pfds[i].revents & POLLIN) {
                ResponseState *state = &connections[i].response;

                char buf[BUF_SIZE];
                ssize_t bytes_read = read(pfds[i].fd, buf, sizeof(buf));

                if (bytes_read > 0) {
                    // Expand the buffer to hold new data
                    char *new_buffer = realloc(state->buffer, state->buffer_size + bytes_read);
                    if (!new_buffer) {
                        fprintf(stderr, "Memory allocation failed\n");
                        continue;
                    }
                    state->buffer = new_buffer;

                    memcpy(state->buffer + state->buffer_size, buf, bytes_read);
                    state->buffer_size += bytes_read;

                    // Process the response(s)
                    test_error_code_t result = process_response(&connections[i]);
                    if (result != TEST_ERROR_NONE) {
                        fprintf(stderr, "Error processing response on connection %d\n", i);
                        close_connection(&connections[i]);
                        pfds[i].fd = -1;
                        continue;
                    }

                    connections[i].last_activity = time(NULL);
                } else if (bytes_read == 0) {
                    // Connection closed by server
                    fprintf(stderr, "Connection %d closed by server\n", i);
                    close_connection(&connections[i]);
                    pfds[i].fd = -1;
                } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    fprintf(stderr, "Read error on connection %d: %s\n",
                            i, strerror(errno));
                    close_connection(&connections[i]);
                    pfds[i].fd = -1;
                }
            }
            if (pfds[i].revents & (POLLERR | POLLHUP)) {
                fprintf(stderr, "Connection %d error or hangup\n", i);
                close_connection(&connections[i]);
                pfds[i].fd = -1;
            }
        }

        // Try to reestablish failed connections
        for (int i = 0; i < num_connections; i++) {
            if (pfds[i].fd == -1) {
                test_error_code_t result = setup_connection(&connections[i], server_addr);
                if (result == TEST_ERROR_NONE) {
                    pfds[i].fd = connections[i].sock_fd;
                    pfds[i].events = POLLIN | POLLOUT;
                    fprintf(stderr, "Reestablished connection %d\n", i);
                }
            }
        }

        queue_pending_requests();

        // Check if we're done
if (resource_count > 0) {
    all_done = true;
    for (int i = 0; i < resource_count; i++) {
        if (resources[i].state == RESOURCE_PENDING) {
            all_done = false;
            break;
        }
    }
    if (all_done) {
        fprintf(stderr, "All resources processed!\n");
    }
}
    }

    // Cleanup
    for (int i = 0; i < num_connections; i++) {
        close_connection(&connections[i]);
    }
    free(connections);
    free(pfds);

    return EXIT_SUCCESS;
}
