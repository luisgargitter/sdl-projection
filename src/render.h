#ifndef RENDER_H
#define RENDER_H

#include "asset.h"
#include "object.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *r;   ///< Pointer to the SDL_Renderer
    int32_t width;     ///< Window width
    int32_t height;    ///< Window height

    float_t near, far; ///< For clipping
    float_t fov_ratio; ///< FOV as ratio of the width or height to the depth of
                       ///< the "camera"
    float_t scaled_fov;

    int32_t num_objects;      ///< Number of objects that are to be rendered
    int32_t capacity_objects; ///< Capacity of the objects array
    object_t *objects;        ///< Vector containing the objects to be rendered

    mat3_t orientation; ///< Global delta orientation (camera angle)
    vec3_t offset;      ///< Global delta offset (camera position)
} render_t;

render_t render_init(SDL_Window *window, float_t fov_ratio);

void render_cleanup(render_t render);

int render_position(render_t *r);

int render_add_object(render_t *render, asset_t *asset, mat3_t orientation,
                      vec3_t offset);

int render_frame(render_t *render);

#endif
