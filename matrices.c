#include "matrices.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int init_matrices(Banker *b) {
    size_t m = b->resources;
    if (!m || !b->customers || b->customers > SIZE_MAX / sizeof(int) / m) {
        fprintf(stderr, "Error: invalid matrix dimensions\n");
        return 0;
    }
    size_t cells = b->customers * m;
    b->allocation = calloc(cells, sizeof(int));
    b->need = malloc(cells * sizeof(int));
    b->work = malloc(m * sizeof(int));
    b->finished = malloc(b->customers);
    if (!b->allocation || !b->need || !b->work || !b->finished) {
        fprintf(stderr, "Error: insufficient memory\n"); return 0;
    }
    memcpy(b->need, b->maximum, cells * sizeof(int));
    return 1;
}
