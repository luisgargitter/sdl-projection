#ifndef SDL_PROJECTION_ERROR_H
#define SDL_PROJECTION_ERROR_H

void info_and_abort2(const char *file, int line, const char *message);

#define info_and_abort(message)                                                \
    info_and_abort2(__FILE__, __LINE__, message)

#endif
