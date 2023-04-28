#include "socket.h"

static void close_sockets(int);

static HttpSocket* all_sockets[255]; // temporary
static int all_sockets_head = 0;

HttpSocket* create_socket(int port) {
    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        ELOG("Socket creation error\n");
        return NULL;
    }
    HttpSocket* httpSocket = malloc(sizeof(HttpSocket));
    httpSocket->sfd = sfd;
    httpSocket->address = calloc(1, sizeof(SAI));
    httpSocket->len = sizeof(*httpSocket->address);
    httpSocket->port = port;
    httpSocket->address->sin_family = AF_INET;
    httpSocket->address->sin_port = htons(httpSocket->port);
    httpSocket->address->sin_addr.s_addr = htonl(INADDR_ANY);
    httpSocket->listeners = malloc(sizeof(struct listeners));
    httpSocket->listeners->map_listeners = new_hash_map();

    /* Pushing socket to stack */
    all_sockets[all_sockets_head++] = httpSocket;
    return httpSocket;
}
void put_listener(HttpSocket* socket, char* path, listener_function listener) {
    put(socket->listeners->map_listeners, path, listener);
}
listener_function get_listener(HttpSocket* socket, char* path) {
    return get(socket->listeners->map_listeners, path);
}
int start(HttpSocket* self) {
    if (bind(self->sfd, (SA *) self->address, self->len) < 0 || listen(self->sfd, BACKLOG) < 0) {
        ELOG("Binding or listening error. Start me later.\n");
        return -1;
    }
    signal(SIGINT, close_sockets);
    printf("Listening on %d\n", self->port);
    while (true) {
        char req_buff[BUFFSZ] = {0};
        int connfd;
        if ((connfd = accept(self->sfd, (SA*) self->address, &self->len)) < 0) {
            ELOG("Read error\n");
            continue;
        }
        size_t n, r = 0;
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
            free_request(req);
            close(connfd);
            continue;
        }
        res = response_function(req);
        write(connfd, res->header, strlen(res->header));
        write(connfd, res->body, strlen(res->body));
        free_request(req);
        free(res); // todo: free response properly
        close(connfd);
    }
    return 0; // should be inaccessible
}
void close_socket(HttpSocket* self) {
    free_map(self->listeners->map_listeners);
    free(self->listeners);
    free(self->address);
    close(self->sfd);
    free(self);
    self = NULL;
}
static void close_sockets(int) {
    ELOG("\nTerminating... I'm sorry.\n");
    for (int i = 0; i < all_sockets_head; ++i) {
        close_socket(all_sockets[i]);
    }
    exit(0);
}