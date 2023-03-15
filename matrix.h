#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#include "Error.h"

typedef struct {
    const uint32_t m;
    const uint32_t n;
    float *mat;
} matrix_t;

inline uint32_t matrixIndex(matrix_t *mat, uint32_t m, uint32_t n);
    // Returns the proper indexvalue as of m and n, 
    // because the matrix is internally a one-dimensional array.
    // This inline function does not check its inputs -> use with caution. 

float matrixRead(matrix_t *mat, uint32_t m, uint32_t n);
    // Arguments are bound-checked.
    // Returns float at given position.
    // Returns NAN when m or n are out of bounds.

Error_t matrixWrite(matrix_t *mat, uint32_t m, uint32_t n, float f);
    // Arguments are bound-checked.
    // Writes f at given position.
    // Returns ERR_OK on success and ERR_EXCEEDS_BOUNDS 
    // when either m or n are out of bounds.

void matrixSet(matrix_t *mat, float **s);
    // Overwrites the previous values.
    // s must have mxn capacity as it overwrites all values.

matrix_t * matrixNew(uint32_t m, uint32_t n, float **s);
    // Returns a Matrix which must be freed with matrixFree() (below) after use.
    // if s is NULL the matrix remains to be later set with matrixSet() (above).

void matrixFree(matrix_t *mat);
    // Frees previously created Matrices with matrixNew() or matrixMultNew() properly.

Error_t matrixMult(matrix_t *m1, matrix_t *m2, matrix_t *res);
    // Dimensions must align, just as we learned in ADM.
    // Returns ERR_EXCEEDS_BOUNDS if dimensions do not align.
    // Returns ERR_NULLPTR if some of the arguments is NULL.
    // Returns ERR_OK on success.
    // 'res' must be allocated and have the proper dimensions aswell.

matrix_t * matrixMultNew(matrix_t *m1, matrix_t *m2);
    // Same behaviour as matrixMult() (above) but stores result
    // in a newly allocated matrix
    //	// Same behaviour as matrixMult() (above) but stores result
    //	in a newly allocated matrix..

#endif
