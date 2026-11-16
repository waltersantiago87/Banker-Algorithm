#ifndef RELEASE_H
#define RELEASE_H

#include "banker.h"
#include <stdio.h>

void release_resources(Banker *b, FILE *out, size_t id, const int *v);

#endif
