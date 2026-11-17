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

#include "request.h"

#include "release.h"

#include "state.h"

#include "parser.h"

#include "commands.h"

int main(int argc, char **argv) {
    Banker b = {0};
    int ok = read_arguments(&b, argc, argv);
    if (ok) ok = read_customers(&b);
    if (ok) ok = init_matrices(&b);
    if (ok) ok = process_commands(&b);
    destroy_banker(&b);
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
