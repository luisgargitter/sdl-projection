#include "matrix.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>

#include "Error.h"


uint32_t matrixIndex(matrix_t *mat, uint32_t m, uint32_t n) {
    return m * mat->n + n;
}

float matrixRead(matrix_t *mat, uint32_t m, uint32_t n) {
    if(m > mat->m || n > mat->n) return NAN;
    return mat->mat[matrixIndex(mat, m, n)];
}

Error_t matrixWrite(matrix_t *mat, uint32_t m, uint32_t n, float f) {
    if(m > mat->m || n > mat->n) return ERR_EXCEEDS_BOUNDS;
    mat->mat[matrixIndex(mat, m, n)] = f;

    return ERR_OK;
}

void matrixSet(matrix_t *mat, float **s) {
    for(uint32_t i = 0; i < mat->m; i++) {
        for(uint32_t j = 0; j < mat->n; j++) {
            mat->mat[matrixIndex(mat, i, j)] = s[i][j];
        }
    } 
}

matrix_t * matrixNew(uint32_t m, uint32_t n, float **s) {
    if(m < 1 || n < 1) return NULL;
    matrix_t *mat = malloc(sizeof(matrix_t));
    if(mat == NULL) abort();
    mat->mat = malloc(sizeof(float) * m * n);
    if(mat->mat == NULL) abort();

    if(s == NULL) return mat;

    matrixSet(mat, s);

    return mat;
}

void matrixFree(matrix_t *mat) {
    free(mat->mat);
    mat->mat = NULL;
    free(mat);
}

Error_t matrixMult(matrix_t *m1, matrix_t *m2, matrix_t *res) {
    if(m1 == NULL || m2 == NULL || res == NULL) return ERR_NULLPTR;
    if(m1->n != m2->m || m1->m != res->m || m2->n != res->n) return ERR_EXCEEDS_BOUNDS;
    
    for(uint32_t i = 0; i < res->m; i++) {
        for(uint32_t j = 0; j < res->n; j++) {
            res->mat[matrixIndex(res, i, j)] = 0.0f;
            for(uint32_t k = 0; k < m1->n; k++) {
                res->mat[matrixIndex(res, i, j)] += m1->mat[matrixIndex(m1, i, k)] * m2->mat[matrixIndex(m2, k, j)];
            }
        }   
    }

    return ERR_OK;
}

matrix_t * matrixMultNew(matrix_t *m1, matrix_t *m2) {
    matrix_t *res = matrixNew(m1->m, m2->n, NULL);
    matrixMult(m1, m2, res);

    return res;
}

