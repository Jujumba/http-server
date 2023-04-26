#include "strings.h"

static void resize(size_t *restrict a, const size_t *restrict b) {
    while (*a <= *b) {
        (*a) *= 2;
    }
}
void concat_strings(char *restrict dst, char *restrict src, size_t *restrict dst_len, char* format) {
    size_t src_len = strlen(src);
    resize(dst_len, &src_len);
    char* buff;
    buff = realloc(dst, *dst_len);
    if (!buff) {
        PANIC("Malloc error\n");
    }
    dst = buff;
    sprintf(dst, format, dst, src);
}