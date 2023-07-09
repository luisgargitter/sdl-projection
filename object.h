//
// Created by luis on 06.07.23.
//

#ifndef SDL_PROJECTION_OBJECT_H
#define SDL_PROJECTION_OBJECT_H

#include "types.h"
#include "asset.h"

typedef struct {
    asset_t* asset;
    vec_3_t* vertices_in_scene;
    SDL_Vertex* proj_v;
    int32_t* visible_faces; // stored as int triplets
    int32_t vf_count;
} object_t;


matrix_3x3_t matrix_3x3_identity();

int apply_mat_3x3(matrix_3x3_t matrix, const vec_3_t* vertices, int32_t num_vertices, vec_3_t* res);

int apply_vec_3(vec_3_t vector, vec_3_t* vertices, int32_t num_vertices, vec_3_t* res);

int update_position(object_t *object, matrix_3x3_t orientation, vec_3_t offset);

int object_init(object_t* object, asset_t* a, matrix_3x3_t orientation, vec_3_t offset);

void object_free(object_t* object);

#endif //SDL_PROJECTION_OBJECT_H
