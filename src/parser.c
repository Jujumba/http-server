#include "parser.h"

hash_map* parse_request(char* request) {
    hash_map* map = new_hash_map();

    size_t req_len = strlen(request);
    size_t head = 0;
    char c;

    char* method = NULL;
    size_t method_len = 0;
    while ((c = request[head++]) != ' ') {
        method = realloc(method, method_len + 1);
        method[method_len++] = c;
    }
    method = realloc(method, method_len + 1);
    method[method_len] = '\0';
    put(map, "method", method);

    char* path = NULL;
    size_t path_len = 0;
    while ((c = request[head++]) != ' ') {
        path = realloc(path, path_len + 1);
        path[path_len++] = c;
    }
    path = realloc(path, path_len + 1);
    path[path_len] = '\0';
    put(map, "path", path);

    char* protocol = NULL;
    size_t protocol_len = 0;
    while ((c = request[head++]) != '\r') {
        protocol = realloc(protocol, protocol_len + 1);
        protocol[protocol_len++] = c;
    }
    protocol = realloc(protocol, protocol_len + 1);
    protocol[protocol_len] = '\0';
    put(map, "protocol", protocol);

    head++;
    while (head < req_len) {
        char* key = NULL;
        size_t key_len = 0;
        while ((c = request[head++]) != ':') {
            key = realloc(key, key_len + 1);
            key[key_len++] = c;
        }
        key = realloc(key, key_len + 1);
        key[key_len] = '\0';

        head++;

        char* value = NULL;
        size_t value_len = 0;
        while ((c = request[head++]) != '\r') {
            value = realloc(value, value_len + 1);
            value[value_len++] = c;
        }
        value = realloc(value, value_len + 1);
        value[value_len] = '\0';

        put(map, key, value);

        head++;
    }

    return map;
}
