#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <stdint.h>

#include "linag.h"
#include "array.h"

/**
 * @brief Asset in memory
 *
 * Contains Mesh and texture of an asset, that can be referenced in a scene.
 */

typedef struct {
    uint32_t vertex[3];  ///< vertex indices
    uint32_t normal[3];  ///< vertex normal indices of corresponding vertex
    uint32_t texture[3]; ///< vertex texture indices of corresponding vertex
} surface_t;

typedef struct {
    int32_t ref_count; ///< Number of times it is being referenced (for
                       ///< automatic deallocation)

    array_t *vertices;
    array_t *normals;
    array_t *uv_coordinates;
    array_t *faces;

    SDL_Texture *t; ///< (optional)
} asset_t;

void asset_load_obj(FILE *file, asset_t *asset);

void asset_free(asset_t *asset);

#endif
