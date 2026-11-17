#include "output.h"

FILE *open_result(void) {
    FILE *out = fopen("result.txt", "w");
    if (!out) perror("result.txt");
    return out;
}

int finish_result(FILE *out, int ok) {
    if (ferror(out)) {
        fprintf(stderr, "Error: writing result.txt\n");
        ok = 0;
    }

    if (fclose(out) != 0) {
        perror("result.txt");
        ok = 0;
    }

    if (!ok && remove("result.txt") != 0)
        perror("Error removing result.txt");

    return ok;
}
