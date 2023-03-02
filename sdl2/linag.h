#ifndef LINAG_H
#define LINAG_H

#include <math.h>
#define DIM 3

typedef struct matrix_t matrix_t;

void matmult(const matrix_t *n1, const matrix_t *n2, matrix_t *res);

void genRotMat(); 

#endif
