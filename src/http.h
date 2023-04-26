#ifndef HTTP_SERVER_HTTP_H
#define HTTP_SERVER_HTTP_H

#include <stddef.h>
#include <sys/stat.h>
#include "misc/iterator.h"
#include "misc/map.h"

typedef unsigned char uchar;
typedef enum {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    I_M_A_TEAPOT = 418, /* <(^.^)> */
} HttpStatus;
typedef struct {
    HttpStatus status;
    char *header, *body;
    size_t header_len, body_len;
} HttpResponse;

HttpResponse* construct_response(HttpStatus status, hash_map* headers, char* file_name);
char* match_status_code(HttpStatus status);

#endif //HTTP_SERVER_HTTP_H