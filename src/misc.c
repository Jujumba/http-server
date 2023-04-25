#include "misc.h"

void panic(char *msg, ...) { // Definitely not stolen from the Rust™
    va_list args;
    va_start(args, msg);
    fprintf(stderr, msg, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
