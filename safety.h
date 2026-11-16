#ifndef SAFETY_H
#define SAFETY_H

#include "banker.h"

int fits(const int *request, const int *limit, size_t m);
int is_safe(Banker *b);

#endif
