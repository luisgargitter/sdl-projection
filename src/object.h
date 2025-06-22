#ifndef SDL_PROJECTION_OBJECT_H
#define SDL_PROJECTION_OBJECT_H

#include "asset.h"
#include "linag.h"

typedef struct {
    asset_t *asset;
    vec3_t offset;
    mat3_t orientation;

    array_t *vertices_in_scene;
    array_t *projected; // SDL_Vertex
    array_t *visible_faces; // stored as int triplets
    array_t *ordered_faces;
} object_t;

int object_init(object_t *object, asset_t *a, mat3_t orientation,
                vec3_t offset);

void object_free(object_t *object);

#endif
