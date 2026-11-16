#include "state.h"
#include "request.h"

void print_state(const Banker *b, FILE *out) {
    size_t m = b->resources;
    fprintf(out, "MAXIMUM | ALLOCATION | NEED\n");
    for (size_t i = 0; i < b->customers; i++) {
        print_vector(out, b->maximum + i * m, m); fprintf(out, " | ");
        print_vector(out, b->allocation + i * m, m); fprintf(out, " | ");
        print_vector(out, b->need + i * m, m); fputc('\n', out);
    }
    fprintf(out, "AVAILABLE "); print_vector(out, b->available, m); fputc('\n', out);
}

