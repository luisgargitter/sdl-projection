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

void mat3x3_rot(vec3_t r, mat3x3_t m) {
	mat3x3_t mr[3];
	mat3x3_rotx(r[0], mr[0]);	
	mat3x3_roty(r[1], mr[1]);
	mat3x3_mul(mr[0], mr[1], mr[2]);
	mat3x3_rotz(r[2], mr[1]);
	mat3x3_mul(mr[2], mr[1], m);
}

void mat3x3_expand(mat3x3_t m3, mat4x4_t m4) {
	for(size_t i = 0; i < 3; i++) {
		for(size_t j = 0; j < 3; j++) {
			m3[i][j] = m4[i][j];
		}
		// zeroing remaining cells.
		m4[i][3] = 0;
		m4[3][i] = 0;	
	}
	m4[3][3] = 1; // identity matrix trait.	
}

void mat4x4_transl(const vec3_t t, matrix4x4_t p) {
	for(size_t i = 0; i < 3; i++) p[i][3] = t[i];
}

void projector(vec3_t offset, vec3_t orientation, mat4x4_t projector) {
	mat3x3_t mr;
	mat3x3_rot(orientation, mr);
	mat3x3_expand(mr, projector);
		
	

}
