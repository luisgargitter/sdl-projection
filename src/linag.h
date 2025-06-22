#ifndef SDL_PROJECTION_LINAG_H
#define SDL_PROJECTION_LINAG_H

#include "asset.h"
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

vec2_t vec2(float x, float y);

vec2_t vec2_add(vec2_t v1, vec2_t v2);
vec2_t vec2_sub(vec2_t v1, vec2_t v2);
vec2_t vec2_mul(vec2_t v, float c);
float vec2_cross(vec2_t v1, vec2_t v2);

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

vec3_t vec3_mul(vec3_t v, float s);

vec3_t vec3_lerp(vec3_t v1, vec3_t v2, float t);

float vec3_length(vec3_t v);

#endif