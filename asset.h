#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "types.h"

/**
 * @brief Asset in memory
 * 
 * Contains Mesh and texture of an asset, that can be referenced in a scene.
 */
typedef struct {
    int32_t v_count;	///< Number of vertices
    vec_3_t* v_vector;	///< Vector, containing the vertices (has length v_count)
    vec_3_t* vn_vector; ///< Vector, specifying the vertices normals (not normalized, has length v_count)

    SDL_Texture* t;     ///< (optional)
    vec_2_t* vt_vector; ///< Vector, specifying the vertices texture coordinates (optional, has length v_count)

    int32_t f_count;	    ///< Number of faces in the mesh
    int32_t* f_vector;	///< Vector, containing the faces (index triplets in v_vector describing triangles)
} asset_t;

int asset_load_obj(FILE* f, asset_t* a);

#endif
