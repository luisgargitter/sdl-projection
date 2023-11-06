#ifndef PROJECTION_H
#define PROJECTION_H

#include "linag.h"

LINAG_DECLARE_VEC(3);

typedef struct {
	float farthest;
	int p1, p2, p3;
} sortable_triangle;

void mat3x3_rotx(float a, mat3x3_t r);

void mat3x3_roty(float a, mat3x3_t r);

void mat3x3_rotz(float a, mat3x3_t r);

#endif
