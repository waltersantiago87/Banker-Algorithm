#define _POSIX_C_SOURCE 200809L
#include "customers.h"
#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

static int parse_customer(const char *p, int *row, size_t m) {
    for (size_t j = 0; j < m; j++) {
        if (!read_int(&p, &row[j])) return 0;
        skip_space(&p);
        if (j + 1 < m && *p++ != ',') return 0;
    }
    return *p == '\0';
}

int read_customers(Banker *b) {
    FILE *f = fopen("customer.txt", "r");
    if (!f) { perror("customer.txt"); return 0; }
    char *line = NULL;
    size_t cap = 0, rows_cap = 0, m = b->resources;
    ssize_t len;
    int ok = 1;
    while ((len = getline(&line, &cap, f)) >= 0) {
        if (b->customers == rows_cap) {
            size_t next = rows_cap ? rows_cap * 2 : 4;
            if (next < rows_cap || next > SIZE_MAX / sizeof(int) / m) {
                fprintf(stderr, "Error: too many customers\n"); ok = 0; break;
            }
            int *tmp = realloc(b->maximum, next * m * sizeof(int));
            if (!tmp) { perror("realloc"); ok = 0; break; }
            b->maximum = tmp; rows_cap = next;
        }
        if (memchr(line, '\0', (size_t)len) ||
            !parse_customer(line, b->maximum + b->customers * m, m)) {
            fprintf(stderr, "Error: customer.txt line %zu: expected %zu comma-separated nonnegative integers\n", b->customers + 1, m);
            ok = 0; break;
        }
        b->customers++;
    }
    if (!feof(f) && ok) { fprintf(stderr, "Error: reading customer.txt\n"); ok = 0; }
    if (!b->customers && ok) { fprintf(stderr, "Error: customer.txt is empty\n"); ok = 0; }
    free(line); fclose(f);
    if (!ok) return 0;
    return 1;
}

