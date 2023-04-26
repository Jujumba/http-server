#include "http.h"

HttpResponse* construct_response(HttpStatus status, hash_map* headers) {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    response->status = status;
    char* status_msg = match_status_code(status);
    // todo: hard-coded
    response->header = malloc(sizeof(uchar) * 80);
    snprintf((char *) response->header, 80, "HTTP/1.1 %d %s\nConnection: Keep-Alive\nContent-Type: application/json\r\n\r\n", status, status_msg);
    response->header_len = strlen((char *) response->header);
    response->header[response->header_len] = '\0';
    return response;
}
char* match_status_code(HttpStatus status) {
    switch (status) {
        case 200:
            return "OK";
        case 400:
            return "Bad Request";
        case 404:
            return "Not Found";
        case 418:
            return "I'm a teapot";
        default:
            return NULL;
    }
}