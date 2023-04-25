#ifndef HTTP_SERVER_MAP_H
#define HTTP_SERVER_MAP_H

#define DEFAULT_CAPACITY 16
#define MAX_CAPACITY (size_t) 1 << 63
#define LOAD_FACTOR 0.75

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "panic.h"

typedef struct bucket_node {
    char* key;
    size_t len;
    void* value;
    struct bucket_node* next;
} bucket_node;
/* Unordered map */
typedef struct hash_map {
    size_t size, capacity;
    bucket_node **buckets;
    void (*put)(struct hash_map*, char*, void*);
    void* (*get)(struct hash_map*, char*);
} hash_map;

hash_map *new_hash_map();
void* get(hash_map*, char*);
void put(hash_map*, char*, void*);
void free_map(hash_map*);

#endif // HTTP_SERVER_MAP_H
