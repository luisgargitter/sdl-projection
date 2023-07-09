#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include "object.h"
#include "asset.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *r;	        ///< Pointer to the SDL_Renderer
    int32_t width;		        ///< Window width
    int32_t height;		        ///< Window height
    float_t fov_ratio;	        ///< FOV as ratio of the width or height to the depth of the "camera"
    int32_t num_objects;        ///< Number of objects that are to be rendered
    int32_t capacity_objects;	///< Capacity of the objects array
    object_t* objects;	        ///< Vector containing the objects to be rendered
} render_t;

int render_init(render_t* render, SDL_Window* window, float_t fov_ratio);

void render_free(render_t* render);

int render_add_object(render_t* render, asset_t* asset, matrix_3x3_t orientation, vec_3_t offset);

int projectObjects(render_t* render);

#endif
