#include "iterator.h"

iterator* new_iterator(hash_map* map) {
    iterator* self = malloc(sizeof(iterator));
    if (!self) {
        PANIC("Malloc error\n");
    }
    *(hash_map *) self->map = *map;
    self->next = next;
    self->index = 0;
    self->node = NULL;
    return self;
}
/* Tough to read coroutine implementation */
const bucket_node* next(iterator* self) {
    if (self->index == 0) {
        goto coroutine_start;
    } else if (self->index >= 1 && self->index < self->map->capacity) {
        self->node = self->node->next;
        goto coroutine_yield;
    } else {
        goto coroutine_end;
    }
coroutine_start:
    for (; self->index < self->map->capacity;) {
        self->node = self->map->buckets[self->index++];
coroutine_yield:
        if (!self->node) goto coroutine_start;
        return self->node;
    }
coroutine_end:
    return NULL;
}