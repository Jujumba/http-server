#ifndef SOCKET_H
#define SOCKET_H

/* Maybe it's worth not to hard code socket's backlog? */
#define BACKLOG 10
#define BUFFSZ  4096

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/stat.h>
#include "misc/map.h"
#include "misc/panic.h"
#include "misc/strings.h"
#include "http.h"

typedef struct sockaddr     SA;
typedef struct sockaddr_in  SAI;
typedef struct listeners {
    hash_map* map_listeners;
} listeners;
typedef struct HttpSocket {
    SAI* address;
    int port, sfd;
    socklen_t len;
    void (*start)(struct HttpSocket* self);
    listeners* listeners;
} HttpSocket;
typedef HttpResponse* (*listener_function)(HttpRequest*);
HttpSocket* create_socket(int port);
void put_listener(HttpSocket* socket, char* path, listener_function listener);
listener_function get_listener(HttpSocket* socket, char* path);
#endif // SOCKET_H