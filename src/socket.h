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
typedef struct HttpSocket {
    SAI* address;
    int port, sfd;
    socklen_t len;
    void (*start)(struct HttpSocket* self);
    hash_map* listeners; /* char* - http path, like / or /home_videos : res_function - function handling a response to that address. todo: RENAME */
} HttpSocket;
typedef HttpResponse* (*res_function)(HttpRequest*);
HttpSocket* create_socket(int port);

#endif // SOCKET_H