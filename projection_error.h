#ifndef SDL_PROJECTION_ERROR_H
#define SDL_PROJECTION_ERROR_H

#ifdef PROJECTION_DEBUG
#define info_and_abort(message) printf("FATAL ERROR in file %s, at line %d%s%s.\n", _FILE__, __LINE__, message == NULL ? "", " with message: ", message == NULL ? "", message)
#else
#define info_and_abort(message) printf("FATAL ERROR OCCURED. PLEASE INFORM DEVELOPERS\n")
#endif

#endif