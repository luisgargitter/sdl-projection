#include "linag.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <math.h>

typedef struct matrix_t {
    uint32_t n;
    uint32_t m;
    float* mem;
} matrix_t;

matrix_t * matrixNew(uint32_t m, uint32_t n, const float **fill) {
    matrix_t *mat = malloc(sizeof(matrix_t));
    if(mat == NULL) return NULL;
    
    mat->n = n;
    mat->m = m;
    mat->mem = malloc(sizeof(float *) * m * n);
    if(mat->mem == NULL) {
        free(mat);
        return NULL;
    }

    if(fill == NULL) return mat;
    uint32_t ind = 0;
    for(uint32_t i = 0; i < m; i++) {
        for(uint32_t j = 0; j < n; j++) {
            mat->mem[ind] = fill[i][j];
            ind++;
        }
    }

    return mat;
}

void matrixFree(matrix_t *m) {
    free(m->mem);
    free(m);
}

int matrixMult(const matrix_t *n1, const matrix_t *n2, matrix_t *res) {
    if(n1->n != n2->m || n1->m != res->m || n2->n != res->n) return -1; // rows and columns need to match
    for(int i = 0; i < n1->m; i++) {
        for(int j = 0; j < n2->n; j++) {
            res->mem[i * res->m + j] = 0.0;
            for(int k = 0; k < n1->n; k++) {
                res->mem[i * res->m + j] += n1->mem[i * n1->m + k] * n2->mem[k * n2->m + j];
            }
        }
    }

    return 0;
}

void matrixDebug(const matrix_t *m) {
    printf("Debug: Matrix at %p (%ux%u):\n", m, m->m, m->n);
    for(int i = 0; i < m->m; i++) {
        for(int j = 0; j < m->n; j++) {
            printf("%f", m->mem[i * m->m + j]);
        }
        printf("\n");
    }
}

#if DIM == 3 // genrotmap only implemented for 3 dimensions
void genrotmat(const float angles[DIM], float matrix[DIM][DIM]) {

}
#endif

