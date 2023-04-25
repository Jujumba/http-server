#ifndef SIMPLE_HTTP_C_PANIC_H
#define SIMPLE_HTTP_C_PANIC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define ELOG(MSG,...)(fprintf(stderr, (MSG), ##__VA_ARGS__))
#define PANIC(FORMAT,...) do { \
    ELOG((FORMAT), ##__VA_ARGS__); \
    exit(EXIT_FAILURE); \
} while(0)

#endif //SIMPLE_HTTP_C_PANIC_H