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

static void print_vector(FILE *out, const int *v, size_t m) {
    for (size_t j = 0; j < m; j++) fprintf(out, "%s%d", j ? " " : "", v[j]);
}

/* sign = 1 allocates; sign = -1 releases or rolls back. */
static void change_allocation(Banker *b, size_t id, const int *v, int sign) {
    for (size_t j = 0; j < b->resources; j++) {
        size_t k = id * b->resources + j;
        b->available[j] -= sign * v[j];
        b->allocation[k] += sign * v[j];
        b->need[k] -= sign * v[j];
    }
}

static void request_resources(Banker *b, FILE *out, size_t id, const int *v) {
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

static void release_resources(Banker *b, FILE *out, size_t id, const int *v) {
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

static void print_state(const Banker *b, FILE *out) {
    size_t m = b->resources;
    fprintf(out, "MAXIMUM | ALLOCATION | NEED\n");
    for (size_t i = 0; i < b->customers; i++) {
        print_vector(out, b->maximum + i * m, m); fprintf(out, " | ");
        print_vector(out, b->allocation + i * m, m); fprintf(out, " | ");
        print_vector(out, b->need + i * m, m); fputc('\n', out);
    }
    fprintf(out, "AVAILABLE "); print_vector(out, b->available, m); fputc('\n', out);
}

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
