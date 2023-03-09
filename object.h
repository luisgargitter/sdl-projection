#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdint.h>
#include "types.h"

#include <SDL2/SDL.h>

typedef struct {
  int32_t vert_c;		///< Number of vertices
  vec_3_t* vert_v;		///< Vector, containing the vertices
  SDL_Vertex* proj_v;	///< Vector, containing the transformed through projection vertices

  int32_t index_c;	///< Size of `index_v`
  int32_t* index_v;	///< Vector, containing the indices of the vertices that need to be rendered (for implementing Z-Buffer)
} object_t;



#endif
