#ifndef LINAG_H
#define LINAG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <error.h>

#include <math.h>

#define LINAG_TEST

#define DIM 3

void matMult(uint32_t m, float** m1, uint32_t n, float** m2, uint32_t r, float** res);

//void genRotMat();

#endif
