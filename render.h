#ifndef RENDER_H
#define RENDER_H

#include "types.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *r;
    int32_t width;
    int32_t height;
    int32_t obj_c;
    object_t* obj_v;
} renderCtx_t;

renderCtx_t * renderCtxNew(SDL_Window* w);

void renderCtxFree(renderCtx_t* r);

void projectObjects(renderCtx_t* r);

#endif
