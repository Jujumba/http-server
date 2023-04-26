#include "strings.h"

static void resize(size_t *restrict a, const size_t *restrict b) {
    while (*a <= *b) {
        (*a) *= 2;
    }
}
void concat_strings(char *restrict dst, char *restrict src) {
    size_t dlen = strlen(dst), slen = strlen(src);
    if (dlen <= slen) {
        char* new = calloc(dlen + slen + 1, sizeof(unsigned char)); // fuck you, realloc
        strcpy(new, dst);
        free(dst);
        dst = new;
    }
    strcat(dst, src);
    dst[dlen + slen] = '\0';
}