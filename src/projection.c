#include "projection.h"
#include "linag.h"

#include <stdlib.h>
#include <stdio.h>

LINAG_USE_VEC(2);
LINAG_USE_VEC(3);
LINAG_USE_VEC(4);

void mat3x3_id(mat3x3_t r)
{
	mat3x3_t m = { 
		{1, 0, 0}, 
		{0, 1, 0}, 
		{0, 0, 1} 
	};

	r = m;
}

void mat3x3_rotx(float a, mat3x3_t r) {
	float c = cosf(a);
	float s = sin(a);
	mat3x3_t m = {
		{1, 0, 0},
		{0, c, -s},
		{0, s, c}
	};

	r = m;
}

void mat3x3_roty(float a, mat3x3_t r) {
	float c = cosf(a);
	float s = sin(a);
	mat3x3_t m = {
		{c, 0, s},
		{0, 1, 0},
		{-s, 0, c}
	};

	r = m;
}

void mat3x3_rotz(float a, mat3x3_t r) {
	float c = cosf(a);
	float s = sin(a);
	mat3x3_t m = {
		{c, -s, 0},
		{s, c, 0},
		{0, 0, 1}
	};

	r = m;
}
