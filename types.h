#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "Error.h"  // the errors should be available everywhere

// shorter name tyes
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;


typedef struct {
    float x, y;
} vec_2_t;

typedef struct {
    float x, y, z;
} vec_3_t;

typedef struct {
    vec_3_t column_vectors[3];
} matrix_3x3_t;

#endif

