#include "socket.h"

static void close_socket(int);
static void start(HttpSocket* self);

HttpSocket* create_socket(int port) {
    HttpSocket* httpSocket = malloc(sizeof(HttpSocket));
    if ((httpSocket->sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        PANIC("Socket creation error\n");
    }
    httpSocket->address = calloc(1, sizeof(SAI));
    httpSocket->len = sizeof(*httpSocket->address);
    httpSocket->port = port;
    httpSocket->address->sin_family = AF_INET;
    httpSocket->address->sin_port = htons(httpSocket->port);
    httpSocket->address->sin_addr.s_addr = htonl(INADDR_ANY);
    httpSocket->start = start;
    httpSocket->listeners = malloc(sizeof(struct listeners));
    httpSocket->listeners->map_listeners = new_hash_map();
    return httpSocket;
}
void put_listener(HttpSocket* socket, char* path, listener_function listener) {
    put(socket->listeners->map_listeners, path, listener);
}
listener_function get_listener(HttpSocket* socket, char* path) {
    return get(socket->listeners->map_listeners, path);
}
static void start(HttpSocket* self) {
    if (bind(self->sfd, (SA *) self->address, self->len) < 0 || listen(self->sfd, BACKLOG) < 0) {
        PANIC("Binding or listening error. Start me later.\n");
    }
    printf("Listening on %d\n", self->port);
    while (true) {
        char req_buff[BUFFSZ] = {0};
        int connfd;
        if ((connfd = accept(self->sfd, (SA*) self->address, &self->len)) < 0) {
            ELOG("Read error\n");
            continue;
        }
        size_t n, r = 0;
        //todo: parse http request properly
        while ((n = read(connfd, req_buff, BUFFSZ)) > 0) {
            r += n;
            if (req_buff[r - 1] == '\n') break;
        }
        HttpRequest* req = parse_request(req_buff); // Assuming that full request fits into `req_buff`
        HttpResponse* res;
        listener_function response_function;
        char* path = get(req->headers, "path");
        if ((response_function = get_listener(self, path)) == NULL) {
            // todo: return 404
            close(connfd);
            continue;
        }
        // todo: unused
        res = response_function(req);
        write(connfd, res->header, res->header_len);
        write(connfd, res->body, res->body_len);
        free_request(req);
        close(connfd);
    }
}
static void close_socket(int) {
    ELOG("\nTerminating... I'm sorry.\n");
    /* todo: close socket */
    exit(0);
}