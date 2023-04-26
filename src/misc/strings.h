#ifndef HTTP_SERVER_STRINGS_H
#define HTTP_SERVER_STRINGS_H

#include <stddef.h>
#include <string.h>
#include "panic.h"

void concat_strings(char *restrict dst, char *restrict src, size_t *restrict dst_len, char* format);

#endif //HTTP_SERVER_STRINGS_H