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
#include "misc/map.h"
#include "misc/panic.h"
#include "misc/strings.h"
#include "http.h"

typedef struct sockaddr     SA;
typedef struct sockaddr_in  SAI;

void create_socket();

#endif // SOCKET_H