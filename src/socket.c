#include "socket.h"

static void shutdown_server(int);
static int response(int, __attribute__((unused)) HttpStatus, char*);
static HttpResponse* construct_response(HttpStatus);
static inline size_t count_bytes(char*);
static void resize(size_t* restrict, const size_t* restrict);
static int sfd;

void create_socket() {
    printf("Creating a socket...\n");
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        PANIC("Socket creation error\n");
    }

    SAI address;
    socklen_t len = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT); 
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (SA *) &address, len) < 0 || listen(sfd, BACKLOG) < 0) {
        PANIC("Binding || Listening error\n");
    }

    printf("Listening on %d\n", PORT);
    signal(SIGINT, shutdown_server);
    while (true) {
        size_t size = BUFFSZ;
        uchar *request = calloc(size, sizeof(uchar)), req_buff[BUFFSZ] = {0};
        int connfd;
        if ((connfd = accept(sfd, (SA*) &address, &len)) < 0) {
            ELOG("Read error\n");
            continue;
        }
        size_t n, r = 0;
        //todo: parse http request properly
        while ((n = read(connfd, req_buff, BUFFSZ)) > 0) {
            r += n;
            if (r >= size) { // For laaaarge requests
                resize(&size, &r);
                request = realloc(request, sizeof(uchar) * size);
                if (!request) {
                    PANIC("Malloc error");
                }
            }
            snprintf((char *) request, size, "%s%s", request, req_buff);
            if (request[r - 2] == '\r' && request[r - 1] == '\n') break;
        }
        printf("%s", request);
        if (response(connfd, OK, "../pages/hello_world.html") == -1) {
            ELOG("Page not found\n");
            continue;
        }
        close(connfd);
        free(request);
    }
}
static void shutdown_server(int) {
    ELOG("\nTerminating... I'm sorry.\n");
    close(sfd);
    exit(0);
}
static int response(int connfd, HttpStatus status, char *file_name) {
    HttpResponse *response = construct_response(status);
    FILE* file_stream = fopen(file_name, "r");
    if (file_stream == NULL) {
        return -1;
    }

    response->body_len = count_bytes(file_name);
    response->body = malloc(sizeof(uchar) * response->body_len + 1);

    fread(response->body, response->body_len, sizeof(uchar), file_stream);
    response->body[response->body_len] = '\0';
    write(connfd, response->header, response->header_len);
    write(connfd, response->body, response->body_len);

    fclose(file_stream);
    free(response->body);
    free(response->header);
    free(response);

    return 0;
}
static HttpResponse* construct_response(HttpStatus status) {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    response->status = status;
    char* status_msg;
    switch (status) {
        case 200:
            status_msg = "OK";
            break;
        case 400:
            status_msg = "Bad Request";
            break;
        case 404:
            status_msg = "Not Found";
            break;
        case 418:
            status_msg = "I'm a teapot";
            break;
    }
    // todo: hard-coded
    response->header = malloc(sizeof(uchar) * 80);
    snprintf((char *) response->header, 80, "HTTP/1.1 %d %s\nConnection: Keep-Alive\nContent-Type: text/html\r\n\r\n", status, status_msg);
    response->header_len = strlen((char *) response->header);
    response->header[response->header_len] = '\0';
    return response;
}
static inline size_t count_bytes(char* file_name) {
    struct stat s;
    if (stat(file_name, &s) == -1) {
        return -1;
    }
    return s.st_size;
}
static void resize(size_t *restrict a, const size_t *restrict b) {
    while (*a <= *b) {
        (*a) *= 2;
    }
}