#include "parser.h"
#include "arguments.h"
#include <ctype.h>
#include <string.h>

/* Command tokens must be separated by whitespace. */
int parse_command(const char *p, int *type, int *id, int *v, size_t m) {
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

