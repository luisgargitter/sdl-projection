#include "linag.h"
#include "types.h"

#include <stdint.h>
#include <stdlib.h>

bool matComp(uint32_t m, uint32_t n, float** m1, float** m2) {
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(m1[i][j] != m2[i][j]) return false;
        }
    }
    return true;
}


void matMult(uint32_t m, float** m1, uint32_t n, float** m2, uint32_t r, float** res) {
    for(int i = 0; i < m; i++) {
        for(uint32_t j = 0; j < r; j++) {
            res[i][j] = 0.0;
            for(uint32_t k = 0; k < n; k++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return;
}

#if DIM == 3 // genrotmap only implemented for 3 dimensions
void genrotmat(const float angles[DIM], float matrix[DIM][DIM]) {

}
#endif
