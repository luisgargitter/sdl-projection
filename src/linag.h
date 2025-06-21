#ifndef SDL_PROJECTION_LINAG_H
#define SDL_PROJECTION_LINAG_H

#include "stdint.h"

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    float m[9];
} mat3_t;

typedef struct {
    float farthest;
    int p1, p2, p3;
} sortable_triangle;

mat3_t mat3_identity();

mat3_t mat3_transpose(mat3_t matrix);

mat3_t mat3_add(mat3_t matrix1, mat3_t matrix2);

mat3_t mat3_matmul(mat3_t matrix1, mat3_t matrix2);

vec3_t mat3_mul(mat3_t matrix, vec3_t vector);

mat3_t mat3_rotation(vec3_t v);

vec3_t vec3(float x, float y, float z);

vec3_t vec3_identity();

vec3_t vec3_add(vec3_t v1, vec3_t v2);

vec3_t vec3_subtract(vec3_t v1, vec3_t v2);

vec3_t vec3_scale(vec3_t v, float s);

vec3_t vec3_lerp(vec3_t v1, vec3_t v2, float t);

vec2_t vec3_map_to_plane(vec3_t v);

float vec3_euclidean_distance(vec3_t v1, vec3_t v2);

// ----- GENERICS -----
/*
#define ladd(X, Y) _Generic((X), mat3_t: mat3_add, vec3_t: vec3_add)(X, Y)

#define lsub(X, Y) _Generic((X), vec3_t: vec3_subtract)(X, Y)

#define lmul(X, Y)                                                             \
    _Generic((Y), mat3_t: mat3_matmul, vec3_t: mat3_apply)(X, Y)

#define leud(X, Y) _Generic((X), vec3_t: vec3_euclidean_distance)(X, Y)
*/
#endif