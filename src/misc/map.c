#include "map.h"

static unsigned hash(char* str, int mask) {
    unsigned result = 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i) {
         result += str[i] + result + (result << 6) + (result >> 31);
         result ^= result >> 6;
    }
    result += result << 3;
    result ^= result >> 6;
    return result & (mask - 1);
}
static bucket_node* new_node(char* key, void* value) {
    bucket_node* node = malloc(sizeof(bucket_node));
    if (!node) {
        fprintf(stderr, "MALLOC ERROR\n");
        exit(1);
    }
    char* copy_key = malloc(strlen(key) + 1);
    strcpy(copy_key, key);
    node->key = copy_key;
    node->value = value;
    node->next = NULL;
    return node;
}
static bucket_node* bucket_contains_node(bucket_node* bucket, bucket_node* single) {
    for (;bucket;bucket = bucket->next) {
        if (strlen(bucket->key) == strlen(single->key) && strcmp(bucket->key, single->key) == 0) {
            return bucket;
        }
    }
    return NULL;
}
static void resize_map(hash_map* map) {
    if (map->capacity >= MAX_CAPACITY) return;
    bucket_node **new_buckets = calloc(map->capacity * 2,sizeof(bucket_node));
    if (!new_buckets) {
        fprintf(stderr, "MALLOC ERROR\n");
        exit(1);
    }
    for (int i = 0; i < map->capacity; ++i) {
        bucket_node* next = NULL;
        for (bucket_node* bucket = map->buckets[i]; bucket; bucket = next) {
            next = bucket->next;
            unsigned new_index = hash(bucket->key, map->capacity * 2);
            bucket->next = new_buckets[new_index];
            new_buckets[new_index] = bucket;
        }
    }
    map->capacity *= 2;
    free(map->buckets);
    map->buckets = new_buckets;
}
void* get(hash_map *map, char *key) {
    unsigned index = hash(key, map->capacity);
    bucket_node *bucket = map->buckets[index];
    for (; bucket; bucket = bucket->next) {
        if (strlen(key) == strlen(bucket->key) && !strcmp(key, bucket->key)) {
            return bucket->value;
        }
    }
    return 0;
}
void put(hash_map* map, char* key, void* value) {
    unsigned index = hash(key, map->capacity);
    bucket_node* node = new_node(key, value), *may_be_in_map = NULL;
    if (!(may_be_in_map = bucket_contains_node(map->buckets[index], node))) { // if map doesn't contain element with `key`
        if (++map->size > (double) map->capacity * LOAD_FACTOR) {
            resize_map(map);
            index = hash(key, map->capacity);
        }
        node->next = map->buckets[index];
        map->buckets[index] = node;
        return;
    }
    free(node);
    may_be_in_map->value = value; // otherwise, update value of existing node
}
hash_map* new_hash_map() {
    hash_map* map = calloc(1, sizeof(hash_map));
    if (!map) {
        error:
        fprintf(stderr, "MALLOC ERROR\n");
        exit(1);
    }
    map->capacity = DEFAULT_CAPACITY;
    map->size = 0;
    map->buckets = calloc(map->capacity, sizeof(bucket_node));
    if (!map->buckets) goto error;
    return map;
}
void free_map(hash_map* map) {
    for (size_t i = 0; i < map->capacity; ++i) {
        bucket_node* next = NULL;
        for (bucket_node* bucket = map->buckets[i]; bucket; bucket = next) {
            next = bucket->next;
            free(bucket->key);
            free(bucket);
        }
    }
    free(map->buckets);
    free(map);
    map = NULL;
}