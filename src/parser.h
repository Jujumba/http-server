#ifndef HTTP_SERVER_PARSER_H
#define HTTP_SERVER_PARSER_H

#include "misc/map.h"
#include <stdlib.h>
#include <string.h>

hash_map* parse_request_to_map(char* request);

#endif //HTTP_SERVER_PARSER_H