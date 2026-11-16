#ifndef BANKER_H
#define BANKER_H

#include <stddef.h>

typedef struct {
    size_t customers, resources;
    int *available, *maximum, *allocation, *need;
    int *work;
    unsigned char *finished;
} Banker;

#endif
