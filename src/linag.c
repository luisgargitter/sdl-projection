#include "linag.h"

#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

vec2_t vec2(float_t x, float_t y) {
    vec2_t v = {x, y};
    return v;
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t v = {a.x + b.x, a.y + b.y};
    return v;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    vec2_t v = {a.x - b.x, a.y - b.y};
    return v;
}

vec2_t vec2_mul(vec2_t a, float c) {
    vec2_t v = {a.x * c, a.y * c};
    return v;
}

float vec2_cross(vec2_t v1, vec2_t v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

mat3_t mat3_identity() {
    mat3_t r = {{1, 0, 0, 0, 1, 0, 0, 0, 1}};

    return r;
}

mat3_t mat3_rotation(vec3_t v) {
    float x = v.x;
    float y = v.y;
    float z = v.z;

    mat3_t r = {
        {cosf(y) * cosf(z), sinf(x) * sinf(y) * cosf(z) - cosf(x) * sinf(z),
         cosf(x) * sinf(y) * cosf(z) + sinf(x) * sinf(z),

         cosf(y) * sinf(z), sinf(x) * sinf(y) * sinf(z) + cosf(x) * cosf(z),
         cosf(x) * sinf(y) * sinf(z) - sinf(x) * cosf(z),

         -sinf(y), sinf(x) * cosf(y), cosf(x) * cosf(y)}};

    return r;
}

mat3_t mat3_add(mat3_t matrix1, mat3_t matrix2) {
    // not implemented yet
    return mat3_identity();
}

mat3_t mat3_matmul(mat3_t matrix1, mat3_t matrix2) {
    mat3_t res;
    for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
            res.m[i * 3 + j] = 0;
            for (int32_t k = 0; k < 3; k++) {
                res.m[i * 3 + j] += matrix1.m[i * 3 + k] * matrix2.m[j + k * 3];
            }
        }
    }

    return res;
}

vec3_t mat3_mul(mat3_t matrix, vec3_t vector) {
    float v[3] = {vector.x, vector.y, vector.z};
    float t[3] = {0, 0, 0};
    for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
            t[i] += matrix.m[i * 3 + j] * v[j];
        }
    }

    vec3_t r = {t[0], t[1], t[2]};
    return r;
}

vec3_t vec3(float x, float y, float z) {
    vec3_t v = {x, y, z};
    return v;
}

// unnecessary, try to get rid of usage, use vec_3(0, 0, 0) instead
vec3_t vec3_identity() {
    vec3_t v = {0, 0, 0};
    return v;
}

vec3_t vec3_add(vec3_t v1, vec3_t v2) {
    vec3_t r = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};

    return r;
}

vec3_t vec3_subtract(vec3_t v1, vec3_t v2) {
    vec3_t r = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};

    return r;
}

vec3_t vec3_mul(vec3_t v, float s) { return vec3(v.x * s, v.y * s, v.z * s); }

/** @brief returns the linear-interpolated Point between v1 and v2 that is part
 * t away from v1.
 * @param v1 Point 1.
 * @param v2 Point 2.
 * @param z value between 0 and 1.
 * @return Point between v1 and v2.
 */
vec3_t vec3_lerp(vec3_t v1, vec3_t v2, float t) {
    return vec3_add(v1, vec3_mul(vec3_subtract(v2, v1), t));
}

float vec3_length(vec3_t v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}