#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

int parse_command(const char *p, int *type, int *id, int *v, size_t m);

#endif
