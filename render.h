#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include "object.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>
/**
 * @brief Holds the data for the renderer context
 * 
 * This struct contains the SDL_Renderer, as well as a vector of all objects in the scene
*/
typedef struct {
    SDL_Renderer *r;	///< Pointer to the SDL_Renderer
    int32_t width;		///< Window width
    int32_t height;		///< Window height
    float_t fov_ratio;	///< FOV as ratio of the width or height to the depth of the "camera"
    int32_t obj_c;		///< Number of objects that are to be rendered
    object_t* obj_v;	///< Vector containing the objects to be rendered
} renderCtx_t;

renderCtx_t* renderCtxNew(SDL_Window* w, int32_t obj_c, float_t fov_ratio);

void renderCtxFree(renderCtx_t* r);

void projectObjects(renderCtx_t* r);

#endif
