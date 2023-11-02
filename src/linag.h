#ifndef SDL_PROJECTION_LINAG_H
#define SDL_PROJECTION_LINAG_H

#include "stdint.h"

typedef struct {
    float x, y;
} vec_2_t;

typedef struct {
    float x, y, z;
} vec_3_t;

typedef struct {
    float m[9];
} matrix_3x3_t;

typedef struct
{
    float farthest;
    int p1, p2, p3;
} sortable_triangle;

matrix_3x3_t matrix_3x3_identity();

matrix_3x3_t matrix_3x3_transpose(matrix_3x3_t matrix);

matrix_3x3_t matrix_3x3_add(matrix_3x3_t matrix1, matrix_3x3_t matrix2);

matrix_3x3_t matrix_3x3_multiply(matrix_3x3_t matrix1, matrix_3x3_t matrix2);

vec_3_t matrix_3x3_apply(matrix_3x3_t matrix, vec_3_t vector);

matrix_3x3_t matrix_3x3_rotation(vec_3_t v);

vec_3_t vec_3(float x, float y, float z);

vec_3_t vec_3_identity();

vec_3_t vec_3_add(vec_3_t v1, vec_3_t v2);

vec_3_t vec_3_subtract(vec_3_t v1, vec_3_t v2);

vec_3_t vec_3_scale(vec_3_t v, float s);

vec_3_t vec_3_lerp(vec_3_t v1, vec_3_t v2, float t);

vec_2_t vec_3_map_to_plane(vec_3_t v);

float vec_3_euclidean_distance(vec_3_t v1, vec_3_t v2);

// ----- GENERICS -----

#define ladd(X, Y) _Generic((X),            \
    matrix_3x3_t: matrix_3x3_add,           \
    vec_3_t: vec_3_add                      \
)(X, Y)

#define lsub(X, Y) _Generic((X),            \
    vec_3_t: vec_3_subtract                 \
)(X, Y)

#define lmul(X, Y) _Generic((Y),            \
    matrix_3x3_t: matrix_3x3_multiply,      \
    vec_3_t: matrix_3x3_apply               \
)(X, Y)

#define leud(X, Y) _Generic((X),            \
    vec_3_t: vec_3_euclidean_distance       \
)(X, Y)

#endif
