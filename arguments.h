#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include "banker.h"
void skip_space(const char **p);
int read_int(const char **p, int *value);
int read_arguments(Banker *b, int argc, char **argv);
#endif
