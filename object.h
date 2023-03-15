#ifndef OBJECT_H
#define OBJECT_H

#include "stack.h"
#include <stdint.h>
#include <SDL2/SDL.h>
#include "types.h"

/**
 * @brief Object in the scene
 * 
 * Contains the vertices, as well as the projected vertices of an object
 */ 
typedef struct {
  int32_t vert_c;		///< Number of vertices
  vec_3_t* vert_v;		///< Vector, containing the vertices
  SDL_Vertex* proj_v;	///< Vector, containing the transformed through projection vertices
  
  surface_t* surface_v;	///< Vector, containing the surfaces of the mesh
  int32_t surface_c;		///< Number of surfaces in the mesh 

  //int32_t index_c;	///< Size of `index_v`
  stack index_st;	///< Stack, containing the indices of the vertices that need to be rendered (for implementing Z-Buffer)
} object_t;
#endif
