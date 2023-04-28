#include "http.h"

void free_request(HttpRequest* self) {
    iterator* it = new_iterator(self->headers);
    const bucket_node* pair;
    while ((pair = it->next(it))) {
        free(pair->value);
    }
    free_map(self->headers);
    free(it);
    free(self);
    self = NULL;
}