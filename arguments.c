#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

void skip_space(const char **p) {
    while (isspace((unsigned char)**p)) ++*p;
}

/* Read a nonnegative integer, rejecting signs, overflow and nonnumeric input. */
int read_int(const char **p, int *value) {
    char *end;
    skip_space(p);
    if (!isdigit((unsigned char)**p)) return 0;
    errno = 0;
    long n = strtol(*p, &end, 10);
    if (errno == ERANGE || n > INT_MAX) return 0;
    *value = (int)n;
    *p = end;
    return 1;
}

int read_arguments(Banker *b, int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Usage: ./banker resource1 [resource2 ...]\n"); return 0; }
    b->resources = (size_t)argc - 1;
    if (b->resources > SIZE_MAX / sizeof(int)) return 0;
    b->available = malloc(b->resources * sizeof(int));
    if (!b->available) { fprintf(stderr, "Error: insufficient memory\n"); return 0; }
    int ok = 1;
    for (size_t j = 0; j < b->resources; j++) {
        const char *p = argv[j + 1];
        if (!read_int(&p, &b->available[j]) || *p != '\0') {
            fprintf(stderr, "Error: invalid resource argument %zu\n", j + 1); ok = 0; break;
        }
    }
    return ok;
}
