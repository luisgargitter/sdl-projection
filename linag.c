#include "linag.h"

#include <stdlib.h>
#include <stdint.h>

matrix_3x3_t matrix_3x3_from_array(float a[9]) {
    vec_3_t i1 = {a[0], a[1], a[2]};
    vec_3_t i2 = {a[3], a[4], a[5]};
    vec_3_t i3 = {a[6], a[7], a[8]};
    matrix_3x3_t m = {i1, i2, i3};
    return m;
}

float[9] matrix_3x3_to_array(matrix_3x3_t *m) {
    
}

matrix_3x3_t matrix_3x3_identity() {
    return matrix_3x3_from_array({
        1, 0, 0, 
        0, 1, 0, 
        0, 0, 1
    });
}

matrix_3x3_t matrix_3x3_transpose(matrix_3x3_t matrix) {
    
    return matrix_3x3_from_array()
}

matrix_3x3_t matrix_3x3_add(matrix_3x3_t matrix1, matrix_3x3_t matrix2);

matrix_3x3_t matrix_3x3_multiply(matrix_3x3_t matrix1, matrix_3x3_t matrix2);

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