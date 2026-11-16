#include "request.h"
#include "safety.h"

void print_vector(FILE *out, const int *v, size_t m) {
    for (size_t j = 0; j < m; j++) fprintf(out, "%s%d", j ? " " : "", v[j]);
}

/* sign = 1 allocates; sign = -1 releases or rolls back. */
void change_allocation(Banker *b, size_t id, const int *v, int sign) {
    for (size_t j = 0; j < b->resources; j++) {
        size_t k = id * b->resources + j;
        b->available[j] -= sign * v[j];
        b->allocation[k] += sign * v[j];
        b->need[k] -= sign * v[j];
    }
}

void request_resources(Banker *b, FILE *out, size_t id, const int *v) {
    size_t m = b->resources;
    if (!fits(v, b->need + id * m, m)) {
        fprintf(out, "The customer %zu request ", id); print_vector(out, v, m);
        fprintf(out, " was denied because exceed its maximum allocation\n");
    } else if (!fits(v, b->available, m)) {
        fprintf(out, "The resources "); print_vector(out, b->available, m);
        fprintf(out, " are not enough to customer %zu request ", id);
        print_vector(out, v, m); fputc('\n', out);
    } else {
        change_allocation(b, id, v, 1);
        if (!is_safe(b)) {
            change_allocation(b, id, v, -1);
            fprintf(out, "The customer %zu request ", id); print_vector(out, v, m);
            fprintf(out, " was denied because result in an unsafe state\n");
        } else {
            fprintf(out, "Allocate to customer %zu the resources ", id);
            print_vector(out, v, m); fputc('\n', out);
        }
    }
}

