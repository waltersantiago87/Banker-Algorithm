#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "banker.h"



#include "arguments.h"

#include "customers.h"
#include "matrices.h"

#include "safety.h"

#include "request.h"

#include "release.h"

#include "state.h"

/* Command tokens must be separated by whitespace. */
static int parse_command(const char *p, int *type, int *id, int *v, size_t m) {
    skip_space(&p);
    if (*p == '*') {
        p++; skip_space(&p); *type = 0; return *p == '\0';
    }
    if (strncmp(p, "RQ", 2) == 0) *type = 1;
    else if (strncmp(p, "RL", 2) == 0) *type = 2;
    else return 0;
    p += 2;
    if (!isspace((unsigned char)*p) || !read_int(&p, id)) return 0;
    for (size_t j = 0; j < m; j++) {
        if (!isspace((unsigned char)*p) || !read_int(&p, &v[j])) return 0;
    }
    skip_space(&p);
    return *p == '\0';
}

static int process_commands(Banker *b) {
    FILE *in = fopen("commands.txt", "r");
    if (!in) { perror("commands.txt"); return 0; }
    int *v = malloc(b->resources * sizeof(int));
    if (!v) { fprintf(stderr, "Error: insufficient memory\n"); fclose(in); return 0; }
    FILE *out = fopen("result.txt", "w");
    if (!out) { perror("result.txt"); free(v); fclose(in); return 0; }
    char *line = NULL;
    size_t cap = 0, number = 0;
    ssize_t len;
    int ok = 1;
    while ((len = getline(&line, &cap, in)) >= 0) {
        int type, id = 0;
        number++;
        if (memchr(line, '\0', (size_t)len) ||
            !parse_command(line, &type, &id, v, b->resources)) {
            fprintf(stderr, "Error: commands.txt line %zu: invalid command or resource count (expected %zu)\n", number, b->resources);
            ok = 0; break;
        }
        if (type && (size_t)id >= b->customers) {
            fprintf(stderr, "Error: commands.txt line %zu: invalid customer ID\n", number);
            ok = 0; break;
        }
        if (type == 0) print_state(b, out);
        else if (type == 1) request_resources(b, out, (size_t)id, v);
        else release_resources(b, out, (size_t)id, v);
    }
    if (!feof(in) && ok) { fprintf(stderr, "Error: reading commands.txt\n"); ok = 0; }
    if (!number && ok) { fprintf(stderr, "Error: commands.txt is empty\n"); ok = 0; }
    if (ferror(out)) { fprintf(stderr, "Error: writing result.txt\n"); ok = 0; }
    if (fclose(out) != 0) { perror("result.txt"); ok = 0; }
    fclose(in); free(line); free(v);
    if (!ok) remove("result.txt");
    return ok;
}

int main(int argc, char **argv) {
    Banker b = {0};
    int ok = read_arguments(&b, argc, argv);
    if (ok) ok = read_customers(&b);
    if (ok) ok = init_matrices(&b);
    if (ok) ok = process_commands(&b);
    destroy_banker(&b);
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
