#ifndef HTTP_SERVER_HTTP_H
#define HTTP_SERVER_HTTP_H

#include <stddef.h>
#include <sys/stat.h>
#include "misc/iterator.h"
#include "parser.h"
#include "misc/map.h"

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
typedef struct HttpRequest {
    hash_map* headers;
} HttpRequest;

HttpRequest* parse_request(char* request);
void free_request(struct HttpRequest* self);

#endif //HTTP_SERVER_HTTP_H