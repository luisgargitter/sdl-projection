#include "linag.h"
#include "types.h"
#include <stdint.h>

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


#ifdef LINAG_TEST
bool matMultTest() {
    // setup --------------------------------------------
    float id[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };
    float m1[3][3] = {
        {2.0, 3.0, 5.0},
        {7.0, 11.0, 13.0},
        {17.0, 19.0, 23.0}
    };
    float res1[3][3];
    float res2[3][3];
    // multiplication with identity matrix --------------
    matMult(3, m1, 3, id, 3, res1);
    // squaring matrix ----------------------------------
    // multiply with different dimensions ---------------

    return true;
}

void linag_testall() {
    float l1[] = {1.0f, 2.0f, 3.0f};
    float l2[] = {1.0f, 2.0f, 3.0f};
    float l3[] = {1.0f, 2.0f, 3.0f};

    float* mem1[] = {l1 ,l2, l3};
    matrix_t* m1 = matrixNew(3, 3, (const float **) mem1);
    matrix_t* res = matrixNew(3, 3, NULL);

    matrixMult(m1, m1, res);
    matrixDebug(res);
}

#endif
