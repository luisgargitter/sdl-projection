#include "linag.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>

matrix_3x3_t matrix_3x3_identity() {
    matrix_3x3_t r = {{
        1, 0, 0, 
        0, 1, 0, 
        0, 0, 1
    }};
    
    return r;
}

matrix_3x3_t matrix_3x3_transpose(matrix_3x3_t matrix) {
    // TODO: transpose matrix

    return matrix;
}

matrix_3x3_t matrix_3x3_rotation(vec_3_t v) {
    float x = v.x;
    float y = v.y;
    float z = v.z;

    matrix_3x3_t r = {{
        cosf(y) * cosf(z), 
        sinf(x) * sinf(y) * cosf(z) - cosf(x) * sinf(z), 
        cosf(x) * sinf(y) * cosf(z) + sinf(x) * sinf(z),
        
        cosf(y) * sinf(z),
        sinf(x) * sinf(y) * sinf(z) + cosf(x) * cosf(z),
        cosf(x) * sinf(y) * sinf(z) - sinf(x) * cosf(z),

        -sinf(y),
        sinf(x) * cosf(y),
        cosf(x) * cosf(y)
    }};

    return r;
}

matrix_3x3_t matrix_3x3_add(matrix_3x3_t matrix1, matrix_3x3_t matrix2) {
    // not implemented yet
    return matrix_3x3_identity();
}

matrix_3x3_t matrix_3x3_multiply(matrix_3x3_t matrix1, matrix_3x3_t matrix2) {
    matrix_3x3_t res;
    for(int32_t i = 0; i < 3; i++) {
        for(int32_t j = 0; j < 3; j++) {
            res.m[i * 3 + j] = 0;
            for(int32_t k = 0; k < 3; k++) {
                res.m[i * 3 + j] += matrix1.m[i * 3 + k] * matrix2.m[j + k * 3];  
            }
        }
    }

    return res;
}

vec_3_t matrix_3x3_apply(matrix_3x3_t matrix, vec_3_t vector) {
    float v[3] = {vector.x, vector.y, vector.z};
    float t[3] = {0, 0, 0};
    for(int32_t i = 0; i < 3; i++) {
        for(int32_t j = 0; j < 3; j++) {
            t[i] += matrix.m[i * 3 + j] * v[j];          
        }
    }

    vec_3_t r = {t[0], t[1], t[2]};
    return r;
}

vec_3_t vec_3(float x, float y, float z) {
    vec_3_t v = {x, y, z};
    return v;
}

vec_3_t vec_3_identity() {
    vec_3_t v = {0, 0, 0};
    return v;
}

vec_3_t vec_3_add(vec_3_t v1, vec_3_t v2) {
    vec_3_t r = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };

    return r;
}

vec_3_t vec_3_subtract(vec_3_t v1, vec_3_t v2) {
    vec_3_t r = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };

    return r;
}

vec_2_t vec_3_map_to_plane(vec_3_t v) {
    vec_2_t v2 = {v.x / v.z, v.y / v.z};
    return v2;
}

int apply_mat_3x3(matrix_3x3_t m, const vec_3_t* vertices, int32_t num_vertices, vec_3_t* res) {
    for(int32_t i = 0; i < num_vertices; i++) {
        res[i] = lmul(m, vertices[i]);
    }

    return 0;
}

int apply_vec_3(vec_3_t v, vec_3_t* vertices, int32_t num_vertices, vec_3_t* res) {
    for(int32_t i = 0; i < num_vertices; i++) {
        res[i] = ladd(v, vertices[i]);
    }

    return 0;
}

float vec_3_euclidean_distance(vec_3_t v1, vec_3_t v2) {
    vec_3_t v = lsub(v1, v2);
    float len = powf(v.x, 2.0) + powf(v.y, 2.0) + powf(v.z, 2.0);
    return sqrtf(len);
}

