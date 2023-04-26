#ifndef HTTP_SERVER_ITERATOR_H
#define HTTP_SERVER_ITERATOR_H

#include <stddef.h>
#include "panic.h"
#include "map.h"

typedef struct iterator {
    size_t index; /* Not intended to be edited manually  */
    hash_map *const map;
    bucket_node *node;
    const bucket_node* (*next)(struct iterator*);
} iterator;

iterator* new_iterator(hash_map*);
const bucket_node* next(iterator*);

#endif //HTTP_SERVER_ITERATOR_H
