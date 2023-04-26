#include "socket.h"

static void shutdown_server(int);
static int response(int, __attribute__((unused)) HttpStatus,hash_map*,char*);
static int sfd;
static hash_map* map_headers;
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
        PANIC("Binding or listening error. Start me later.\n");
    }

    printf("Listening on %d\n", PORT);
    signal(SIGINT, shutdown_server);
    map_headers = new_hash_map();
    map_headers->put(map_headers, "Content-Type", "application/json");
    map_headers->put(map_headers, "Connection", "Keep-Alive");
    while (true) {
        char req_buff[BUFFSZ] = {0};
        int connfd;
        if ((connfd = accept(sfd, (SA*) &address, &len)) < 0) {
            ELOG("Read error\n");
            continue;
        }
        size_t n, r = 0;
        //todo: parse http request properly
        while ((n = read(connfd, req_buff, BUFFSZ)) > 0) {
            r += n;
            if (req_buff[r - 1] == '\n') break;
        }
        response(connfd, OK, map_headers, "../pages/package.json");
        close(connfd);
    }
}
static void shutdown_server(int) {
    ELOG("\nTerminating... I'm sorry.\n");
    free_map(map_headers);
    close(sfd);
    exit(0);
}
static int response(int connfd, HttpStatus status, hash_map* headers,char *file_name) {
    HttpResponse *response = construct_response(status, headers, file_name);

    write(connfd, response->header, response->header_len);
    write(connfd, response->body, response->body_len);

    free(response->body);
     free(response->header);
    free(response);

    return 0;
}