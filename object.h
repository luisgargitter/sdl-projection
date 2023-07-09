//
// Created by luis on 06.07.23.
//

#ifndef SDL_PROJECTION_OBJECT_H
#define SDL_PROJECTION_OBJECT_H

#include "types.h"
#include "asset.h"

typedef struct {
    asset_t* asset;
    matrix_3x3_t scene_orientation;
    vec_3_t scene_offset;
    SDL_Vertex* proj_v;
    int32_t* visible_faces; // stored as int triplets
    int32_t vf_count;
} object_t;

#endif //SDL_PROJECTION_OBJECT_H
