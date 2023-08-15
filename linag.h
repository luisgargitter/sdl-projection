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
    vec_3_t column_vectors[3];
} matrix_3x3_t;

matrix_3x3_t matrix_3x3_from_array(float array[9]);

matrix_3x3_t matrix_3x3_identity();

matrix_3x3_t matrix_3x3_transpose(matrix_3x3_t matrix);

matrix_3x3_t matrix_3x3_add(matrix_3x3_t matrix1, matrix_3x3_t matrix2);

matrix_3x3_t matrix_3x3_multiply(matrix_3x3_t matrix1, matrix_3x3_t matrix2);

vec_3_t matrix_3x3_apply(matrix_3x3_t matrix, vec_3_t vector);

matrix_3x3_t matrix_3x3_rotation(float x, float y, float z);

int apply_mat_3x3(matrix_3x3_t matrix, const vec_3_t* vertices, int32_t num_vertices, vec_3_t* res);

int apply_vec_3(vec_3_t vector, vec_3_t* vertices, int32_t num_vertices, vec_3_t* res);

vec_3_t vec_3_add(vec_3_t v1, vec_3_t v2);

#endif
