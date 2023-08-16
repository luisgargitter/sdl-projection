#include "projection_error.h"

#include <stdlib.h>
#include <stdio.h>

void info_and_abort_debug(const char* message) {
    printf("FATAL ERROR in file %s, at line %d%s%s.\n", 
        __FILE__, __LINE__, 
        message == NULL ? "" : " with message: ", 
        message == NULL ? "" : message);
    abort();
}

void info_and_abort_prod(const char* message) {
    printf("FATAL ERROR OCCURED (MESSAGE: %s). PLEASE INFORM DEVELOPERS\n", message);
    abort();
}