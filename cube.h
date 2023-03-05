#ifndef CUBE_H
#define CUBE_H

#include "types.h"
#include <stdint.h>

object_t* cubeNew(float edgelen);

void cubeFree(object_t* cube);

void objectMove(object_t* obj, vec_3_t off);

#endif
