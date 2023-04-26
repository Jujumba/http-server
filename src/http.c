#include "http.h"

typedef bucket_node pair;

static inline size_t count_bytes(char* file_name) {
    struct stat s;
    if (stat(file_name, &s) == -1) {
        return -1;
    }
    return s.st_size;
}
static void read_file(HttpResponse* self,char* file_name) {
    FILE* file_stream = fopen(file_name, "r");
    if (file_stream == NULL) {
        PANIC("File %s doesn't exist!", file_name);
    }

    self->body_len = count_bytes(file_name);
    self->body = malloc(sizeof(uchar) * self->body_len + 1);

    fread(self->body, self->body_len, sizeof(uchar), file_stream);
    self->body[self->body_len] = '\0';
    fclose(file_stream);
}
static void construct_header(HttpResponse* self, hash_map* headers) {
    iterator* it = new_iterator(headers);

    self->header_len = 4096; // todo: hard-code. realloc string dynamically
    self->header = malloc(self->header_len);
    sprintf(self->header, "HTTP/1.1 %d %s", self->status, match_status_code(self->status));
    const pair* pair;
    while ((pair = it->next(it))) {
        sprintf(self->header, "%s\n%s: %s", self->header, pair->key, (char *)pair->value);
    }
    strcat(self->header, "\r\n\r\n");
    self->header_len = strlen(self->header);
    free(it);
}
HttpResponse* construct_response(HttpStatus status, hash_map* headers, char* file_name) {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    response->status = status;
    construct_header(response, headers);
    read_file(response, file_name);
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