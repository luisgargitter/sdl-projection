#include "render.h"
#include "types.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>

Error_t renderCtxNew(SDL_Window *w, renderCtx_t *p, int32_t obj_c) {
    if(p == NULL) return ERR_NULLPTR;
    p->r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if(p->r == NULL) {
        free(p);
        return ERR_NULLPTR;
    }
    SDL_GetWindowSize(w, &p->width, &p->height);

    p->obj_c = obj_c;
    p->obj_v = malloc(sizeof(object_t) * p->obj_c);
    
    return ERR_OK;
}

void renderCtxFree(renderCtx_t* p) {
    SDL_DestroyRenderer(p->r);
    if(p->obj_c > 0) free(p->obj_v);
    p->obj_c = 0;
    free(p);
}

void projectCentral(renderCtx_t* r) {
    object_t* o = NULL;
    for(int32_t i = 0; i < r->obj_c; i++) {
        o = r->obj_v + i;
        for(int32_t j = 0; j < o->vert_c; j++) {
            o->proj_v[j].position.x = (o->vert_v[j].x / powf(o->vert_v[j].z, 0.5)) * 50 + ((float) r->width / 2);
            o->proj_v[j].position.y = (o->vert_v[j].y / powf(o->vert_v[j].z, 0.5)) * 50 + ((float) r->height / 2);
        }
    }
}

void applyColor(renderCtx_t* r) {   // temp function for testing. will be replaced by shading function in future
    static int i = 0;
    if(i != 0) return;
    i = 1;
    uint8_t colors[3] = {0, 127, 255};
    srand(time(NULL));
    for(int32_t i = 0; i < r->obj_c; i++) {
        object_t* o = r->obj_v + i;
        for(int32_t j = 0; j < o->vert_c; j++) {
            o->proj_v[j].color.r = colors[rand()%3];
            o->proj_v[j].color.g = colors[rand()%3];
            o->proj_v[j].color.b = colors[rand()%3];
            o->proj_v[j].color.a = 255;
        }
    }
}

void renderScene(renderCtx_t* r) {
    SDL_SetRenderDrawColor(r->r, 0, 0, 0, 255);
    SDL_RenderClear(r->r);
    //SDL_SetRenderDrawColor(r->r, 255, 255, 255, 255);
    object_t* t = NULL;
    for(int32_t i = 0; i < r->obj_c; i++) {
        t = r->obj_v + i;
        SDL_RenderGeometry(r->r, NULL, t->proj_v, t->vert_c, t->index_v, t->index_c);
    }
    SDL_RenderPresent(r->r);
}

void projectObjects(renderCtx_t* r) {
    projectCentral(r);
    applyColor(r);
    renderScene(r);
}
