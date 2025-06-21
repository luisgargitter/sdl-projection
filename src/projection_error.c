#include "projection_error.h"

#include <stdio.h>
#include <stdlib.h>

void info_and_abort_debug(const char *file, int line, const char *message) {
    printf("FATAL ERROR in file %s, at line %d%s%s.\n", file, line,
           message == NULL ? "" : " with message: ",
           message == NULL ? "" : message);
    abort();
}

void info_and_abort_prod(const char *message) {
    printf("FATAL ERROR OCCURED (MESSAGE: %s). PLEASE INFORM DEVELOPERS\n",
           message);
    abort();
}
