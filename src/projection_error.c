#include "projection_error.h"

#include <stdio.h>
#include <stdlib.h>

void info_and_abort2(const char *file, int line, const char *message) {
    printf("FATAL ERROR in file %s, at line %d%s%s.\n", file, line,
           message == NULL ? "" : " with message: ",
           message == NULL ? "" : message);
    abort();
}
