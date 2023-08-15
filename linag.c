#include "linag.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>

matrix_3x3_t matrix_3x3_from_array(float a[9]) {
    vec_3_t c1 = {a[0], a[1], a[2]};
    vec_3_t c2 = {a[3], a[4], a[5]};
    vec_3_t c3 = {a[6], a[7], a[8]};
    matrix_3x3_t m = {c1, c2, c3};
    return m;
}

void matrix_3x3_to_array(matrix_3x3_t m, float *f) {
    for(int32_t i = 0; i < 3; i++) {
        f[i * 3 + 0] = m.column_vectors[i].x;
        f[i * 3 + 1] = m.column_vectors[i].y;
        f[i * 3 + 2] = m.column_vectors[i].z;
    }
}

matrix_3x3_t matrix_3x3_identity() {
    float f[] = {
        1, 0, 0, 
        0, 1, 0, 
        0, 0, 1
    };
    return matrix_3x3_from_array(f);
}

matrix_3x3_t matrix_3x3_transpose(matrix_3x3_t matrix) {
    float f[9];
    matrix_3x3_to_array(matrix, f);

    // TODO: transpose matrix

    return matrix_3x3_from_array(f);
}

matrix_3x3_t matrix_3x3_rotation(float x, float y, float z) {
    float f[9] = {
        cosf(y) * cosf(z), 
        sinf(x) * sinf(y) * cosf(z) - cosf(x) * sinf(z), 
        cosf(x) * sinf(y) * cosf(z) + sinf(x) * sinf(z),
        
        cosf(y) * sinf(z),
        sinf(x) * sinf(y) * sinf(z) + cosf(x) * cosf(z),
        cosf(x) * sinf(y) * sinf(z) - sinf(x) * cosf(z),

        -sinf(y),
        sinf(x) * cosf(y),
        cosf(x) * cosf(y)
    };

    return matrix_3x3_from_array(f);
}

matrix_3x3_t matrix_3x3_add(matrix_3x3_t matrix1, matrix_3x3_t matrix2) {
    return matrix_3x3_identity();
}

matrix_3x3_t matrix_3x3_multiply(matrix_3x3_t matrix1, matrix_3x3_t matrix2) {
    float m1[9], m2[9], res[9];
    matrix_3x3_to_array(matrix1, m1);
    matrix_3x3_to_array(matrix2, m2);

    for(int32_t i = 0; i < 3; i++) {
        for(int32_t j = 0; j < 3; j++) {
            res[i * 3 + j] = 0;
            for(int32_t k = 0; k < 3; k++) {
                res[i * 3 + j] += m1[i * 3 + k] * m2[j + k * 3];  
            }
        }
    }

    return matrix_3x3_from_array(res);
}

vec_3_t matrix_3x3_apply(matrix_3x3_t matrix, vec_3_t vector);


int apply_mat_3x3(matrix_3x3_t m, const vec_3_t* vertices, int32_t num_vertices, vec_3_t* res) {
    for(int32_t i = 0; i < num_vertices; i++) {
        res[i].x = m.column_vectors[0].x * vertices[i].x + m.column_vectors[1].x * vertices[i].y + m.column_vectors[2].x * vertices[i].z;
        res[i].y = m.column_vectors[0].y * vertices[i].x + m.column_vectors[1].y * vertices[i].y + m.column_vectors[2].y * vertices[i].z;
        res[i].z = m.column_vectors[0].z * vertices[i].x + m.column_vectors[1].z * vertices[i].y + m.column_vectors[2].z * vertices[i].z;
    }

    return 0;
}

int apply_vec_3(vec_3_t v, vec_3_t* vertices, int32_t num_vertices, vec_3_t* res) {
    for(int32_t i = 0; i < num_vertices; i++) {
        res[i].x = v.x + vertices[i].x;
        res[i].y = v.y + vertices[i].y;
        res[i].z = v.z + vertices[i].z;
    }

    return 0;
}

vec_3_t vec_3_add(vec_3_t v1, vec_3_t v2) {
    vec_3_t r;
    r.x = v1.x + v2.x;
    r.y = v1.y + v2.y;
    r.z = v1.z + v2.z;

    return r;
}
