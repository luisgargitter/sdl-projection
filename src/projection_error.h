#ifndef SDL_PROJECTION_ERROR_H
#define SDL_PROJECTION_ERROR_H

void info_and_abort_debug(const char *file, int line, const char *message);

void info_and_abort_prod(const char *message);

// #ifdef PROJECTION_DEBUG
#define info_and_abort(message)                                                \
    info_and_abort_debug(__FILE__, __LINE__, message)
// #else
// #define info_and_abort(message) info_and_abort_prod(message)
// #endif

#endif
