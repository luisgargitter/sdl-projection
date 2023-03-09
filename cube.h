#ifndef CUBE_H
#define CUBE_H

#include "types.h"
#include <stdint.h>

Error_t cubeNew(float edgelen, object_t* o);

void cubeFree(object_t* cube);

void objectMove(object_t* obj, vec_3_t off);

#endif
