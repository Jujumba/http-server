#include "strings.h"

static void resize(size_t *restrict a, const size_t *restrict b) {
    while (*a <= *b) {
        (*a) *= 2;
    }
}
void concat_strings(char *restrict dst, char *restrict src, size_t *restrict dst_len) {
    size_t src_len = strlen(src);
    resize(dst_len, src_len);
    dst = realloc(dst, *dst_len);
    if (!dst) {
        PANIC("Malloc error\n");
    }
    snprintf(src, *src_len, "%s%s", src, dst);
}