#ifndef HTTP_SERVER_MAP_H
#define HTTP_SERVER_MAP_H

#define DEFAULT_CAPACITY 16
#define MAX_CAPACITY (size_t) 1 << 63
#define LOAD_FACTOR 0.75

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct bucket_node {
    char* key;
    void * value;
    struct bucket_node* next;
} bucket_node;
typedef struct hash_map {
    size_t size, capacity;
    bucket_node **buckets;
} hash_map;

hash_map *new_hash_map();
void* get(hash_map*, char*);
void put(hash_map*, char*, void*);

#endif // HTTP_SERVER_MAP_H