#include "linag.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <math.h>

typedef struct matrix_t {
    uint32_t n;
    uint32_t m;
    float** mem_v;
} matrix_t;

void matmult(const float matrix[DIM][DIM], const float vector[DIM], float out[DIM]) {
    float acc[3] = {0.0, 0.0, 0.0}; 
        // performancewise not the best option
        // but dont want to take memory responsibility;
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            acc[i] += matrix[i][j] * vector[j];
        }
    }
    for(int i = 0; i < DIM; i++) {
        out[i] = acc[i];
    }
}

#if DIM == 3 // genrotmap only implemented for 3 dimensions
void genrotmat(const float angles[DIM], float matrix[DIM][DIM]) {

}
#endif

