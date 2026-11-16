#include "release.h"
#include "request.h"
#include "safety.h"

void release_resources(Banker *b, FILE *out, size_t id, const int *v) {
    size_t m = b->resources;
    if (!fits(v, b->allocation + id * m, m)) {
        fprintf(out, "The customer %zu release ", id); print_vector(out, v, m);
        fprintf(out, " was denied because exceed its current allocation\n");
        return;
    }
    change_allocation(b, id, v, -1);
    fprintf(out, "Release from customer %zu the resources ", id);
    print_vector(out, v, m); fputc('\n', out);
}

