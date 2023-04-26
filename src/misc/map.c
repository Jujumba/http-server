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
        PANIC("MALLOC ERROR\n");
    }
    char* copy_key = malloc(strlen(key) + 1);
    strcpy(copy_key, key);
    node->key = copy_key;
    node->value = value;
    node->next = NULL;
    node->len = strlen(key);
    return node;
}
static bucket_node* bucket_contains_node(bucket_node* bucket, bucket_node* single) {
    for (;bucket;bucket = bucket->next) {
        if (bucket->len == single->len && strcmp(bucket->key, single->key) == 0) {
            return bucket;
        }
    }
    return NULL;
}
static void resize_map(hash_map* self) {
    if (self->capacity >= MAX_CAPACITY) return;
    bucket_node **new_buckets = calloc(self->capacity * 2,sizeof(bucket_node));
    if (!new_buckets) {
        PANIC("MALLOC ERROR\n");
    }
    for (int i = 0; i < self->capacity; ++i) {
        bucket_node* next = NULL;
        for (bucket_node* bucket = self->buckets[i]; bucket; bucket = next) {
            next = bucket->next;
            unsigned new_index = hash(bucket->key, self->capacity * 2);
            bucket->next = new_buckets[new_index];
            new_buckets[new_index] = bucket;
        }
    }
    self->capacity *= 2;
    free(self->buckets);
    self->buckets = new_buckets;
}
void* get(hash_map *self, char *key) {
    unsigned index = hash(key, self->capacity);
    bucket_node *in_bucket, node;
    node.key = key; // manually create a node on the stack (!)
    node.len = strlen(node.key);
    if ((in_bucket = bucket_contains_node(self->buckets[index], &node))) {
        return in_bucket->value;
    }
    return NULL;
}
void put(hash_map* self, char* key, void* value) {
    unsigned index = hash(key, self->capacity);
    bucket_node* node = new_node(key, value), *may_be_in_map = NULL;
    if (!(may_be_in_map = bucket_contains_node(self->buckets[index], node))) { // if map doesn't contain element with `key`
        if (++self->size > (double) self->capacity * LOAD_FACTOR) {
            resize_map(self);
            index = hash(key, self->capacity);
        }
        node->next = self->buckets[index];
        self->buckets[index] = node;
        return;
    }
    free(node);
    may_be_in_map->value = value; // otherwise, update value of existing node
}
hash_map* new_hash_map() {
    hash_map* map = calloc(1, sizeof(hash_map));
    if (!map) {
        PANIC("MALLOC ERROR\n");
    }
    map->capacity = DEFAULT_CAPACITY;
    map->size = 0;
    map->buckets = calloc(map->capacity, sizeof(bucket_node));
    map->put = put;
    map->get = get;
    map->free_map = free_map;
    if (!map->buckets) {
        PANIC("MALLOC ERROR\n");
    }
    return map;
}
void free_map(hash_map* self) {
    for (size_t i = 0; i < self->capacity; ++i) {
        bucket_node* next = NULL;
        for (bucket_node* bucket = self->buckets[i]; bucket; bucket = next) {
            next = bucket->next;
            free(bucket->key);
            free(bucket);
        }
    }
    free(self->buckets);
    free(self);
    self = NULL;
}
