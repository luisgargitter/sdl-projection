#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "Error.h"  // the errors should be available everywhere

/* shorter name tyes */
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;

/**
 * @brief A 3D vector
*/
typedef struct
{
  float x;
  float y;
  float z;

} vec_3_t; 

typedef struct 
{
  vec_3_t colVec_v[3];
} matrix_3x3_t;

/**
 * @brief Object in the scene
 * 
 * Contains the vertices, as well as the projected vertices of an object
 */ 
typedef struct {
  int32_t vert_c;		///< Number of vertices
  vec_3_t* vert_v;		///< Vector, containing the vertices
  SDL_Vertex* proj_v;	///< Vector, containing the transformed through projection vertices

  int32_t index_c;	///< Size of `index_v`
  int32_t* index_v;	///< Vector, containing the indices of the vertices that need to be rendered (for implementing Z-Buffer)
} object_t;


#endif

