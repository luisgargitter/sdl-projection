#ifndef LINAG_H
#define LINAG_H

#include <float.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>

#ifdef LINAG_H_INLINE
#define LINAG_H_FUNC static inline
#else
#define LINAG_H_FUNC static
#endif

#define LINAG_DECLARE_VEC(n);												\
typedef float vec##n##_t[n];												\
																			\
typedef vec##n##_t mat##n##x##n##_t[n];										\

#define LINAG_USE_VEC(n);													\
LINAG_DECLARE_VEC(n);														\
																			\
LINAG_H_FUNC void vec##n##_add(												\
	const vec##n##_t v1, const vec##n##_t v2, vec##n##_t r) { 				\
	for(size_t i = 0; i < n; i++) 											\
		r[i] = v1[i] + v2[i];												\
}																			\
																			\
LINAG_H_FUNC void vec##n##_sub(												\
	const vec##n##_t v1, const vec##n##_t v2, vec##n##_t r) { 				\
	for(size_t i = 0; i < n; i++)											\
		r[i] = v1[i] - v2[i];												\
}																			\
																			\
LINAG_H_FUNC void vec##n##_scale(											\
	const vec##n##_t v, float f, vec##n##_t r) {							\
	for(size_t i = 0; i < n; i++) 											\
		r[i] = f * v[i];													\
}																			\
																			\
LINAG_H_FUNC float vec##n##_euclid_dist(									\
	const vec##n##_t v1, const vec##n##_t v2) {								\
    vec##n##_t v;															\
	vec##n##_sub(v1, v2, v);												\
	float acc = 0;															\
	for(size_t i = 0; i < n; i++) 											\
		acc += v[i];														\
    return sqrtf(acc);														\
}																			\
																			\
LINAG_H_FUNC float vec##n##_innerp(											\
	const vec##n##_t v1, const vec##n##_t v2) { 							\
	float acc = 0;															\
	for(size_t i = 0; i < n; i++) 											\
		acc += v1[i] * v2[i];												\
	return acc;																\
}																			\
																			\
LINAG_H_FUNC void vec##n##_outerp(											\
	const vec##n##_t v1, const vec##n##_t v2, mat##n##x##n##_t r) {			\
	for(size_t i = 0; i < n; i++)											\
		for(size_t j = 0; j < n; j++) 										\
			r[i][j] = v1[i] * v2[j];										\
}																			\
																			\
LINAG_H_FUNC void vec##n##_lerp(											\
	const vec##n##_t v1, const vec##n##_t v2, float f, vec##n##_t r) {		\
	vec##n##_sub(v2, v1, r);												\
	vec##n##_scale(r, f, r);												\
	vec##n##_add(v1, r, r);													\
}																			\
																			\
LINAG_H_FUNC void vec##n##_matp(											\
	const mat##n##x##n##_t m, const vec##n##_t v, vec##n##_t r) {			\
	for(size_t i = 0; i < n; i++) {											\
		r[i] = 0;															\
		for(size_t j = 0; j < n; j++)										\
			r[i] += m[j][i] * v[j];											\
	}																		\
}																			\
																			\
LINAG_H_FUNC void mat##n##x##n##_mul(										\
	const mat##n##x##n##_t m1, const mat##n##x##n##_t m2,	 				\
	mat##n##x##n##_t r) {													\
	for(size_t i = 0; i < n; i++) {											\
		vec##n##_matp(m1, m2[i], r[i]);										\
	}																		\
}																			\

#endif
