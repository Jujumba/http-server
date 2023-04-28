#include "socket.h"

static void close_sockets(int);
static SAI* get_raw_socket(int);

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
    httpSocket->port = port;
    httpSocket->address = get_raw_socket(httpSocket->port);
    httpSocket->len = sizeof(*httpSocket->address);
    httpSocket->listeners = malloc(sizeof(struct listeners_map));
    httpSocket->listeners->map_listeners = new_hash_map();

    /* Pushing socket to stack */
    all_sockets[all_sockets_head++] = httpSocket;
    return httpSocket;
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
        handler handler_function;
        char* path = get(req->headers, "path");
        if ((handler_function = get_listener(self, path)) == NULL) {
            // todo: return 404
            free_request(req);
            close(connfd);
            continue;
        }
        res = handler_function(req);
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
static SAI* get_raw_socket(int port) {
    SAI* sai = malloc(sizeof (SAI));
    sai->sin_family = AF_INET;
    sai->sin_port = htons(port);
    sai->sin_addr.s_addr = htonl(INADDR_ANY);
    return sai;
}