#include "http.h"

void free_request(HttpRequest* self) {
    iterator* it = new_iterator(self->map);
    const bucket_node* pair;
    while ((pair = it->next(it))) {
        free(pair->value);
    }
    free_map(self->map);
    free(it);
    free(self);
    self = NULL;
}