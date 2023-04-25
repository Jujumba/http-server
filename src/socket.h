#ifndef SOCKET_H
#define SOCKET_H

#define BACKLOG 10
#define BUFFSZ  4096
#define PORT    8888

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/stat.h>
#include "misc/panic.h"

typedef unsigned char       uchar;
typedef struct sockaddr     SA;
typedef struct sockaddr_in  SAI;

typedef enum {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    I_M_A_TEAPOT = 418, /* <(^.^)> */
} HttpStatus;
typedef struct {
    HttpStatus status;
    unsigned char *header, *body;
    size_t header_len, body_len;
} HttpResponse;
void create_socket();

#endif // SOCKET_H
