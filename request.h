#ifndef REQUEST_H
#define REQUEST_H

#include "banker.h"
#include <stdio.h>

void print_vector(FILE *out, const int *v, size_t m);
void change_allocation(Banker *b, size_t id, const int *v, int sign);
void request_resources(Banker *b, FILE *out, size_t id, const int *v);

#endif
