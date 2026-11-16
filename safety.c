#include "safety.h"
#include <string.h>

int fits(const int *request, const int *limit, size_t m) {
    for (size_t j = 0; j < m; j++)
        if (request[j] > limit[j]) return 0;
    return 1;
}

/* Safety test: simulate each customer finishing and returning its allocation. */
int is_safe(Banker *b) {
    size_t m = b->resources, done = 0;
    memcpy(b->work, b->available, m * sizeof(int));
    memset(b->finished, 0, b->customers);
    while (done < b->customers) {
        int progress = 0;
        for (size_t i = 0; i < b->customers; i++) {
            if (!b->finished[i] && fits(b->need + i * m, b->work, m)) {
                for (size_t j = 0; j < m; j++)
                    b->work[j] += b->allocation[i * m + j];
                b->finished[i] = 1;
                done++; progress = 1;
            }
        }
        if (!progress) return 0;
    }
    return 1;
}

