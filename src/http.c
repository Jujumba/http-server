#include "http.h"

HttpRequest* parse_request(char* request) {
    HttpRequest* self = malloc(sizeof(HttpRequest));
    self->headers = parse_request_to_map(request);
    return self;
}

void free_request(HttpRequest* self) {
    iterator* it = new_iterator(self->headers);
    const bucket_node* pair;
    while ((pair = next(it))) {
        free(pair->value);
    }
    free_map(self->headers);
    free(it);
    free(self);
    self = NULL;
}