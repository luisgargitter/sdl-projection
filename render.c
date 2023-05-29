#include "render.h"
#include "types.h"
#include "object.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>

renderCtx_t* renderCtxNew(SDL_Window *w, int32_t obj_c, float_t fov) {
    renderCtx_t* p = malloc(sizeof(renderCtx_t));
    if(p == NULL) return NULL;
    p->r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if(p->r == NULL) {
        free(p);
        return NULL;
    }
    SDL_GetWindowSize(w, &p->width, &p->height);

    p->obj_c = obj_c;
    p->obj_v = malloc(sizeof(object_t) * p->obj_c);
    
    p->fov_ratio = tanf(fov / 2.0);
    
    return p;
}

void renderCtxFree(renderCtx_t* p) {
    SDL_DestroyRenderer(p->r);
    if(p->obj_c > 0) free(p->obj_v);
    p->obj_c = 0;
    free(p);
}

void projectCentral(renderCtx_t* r) {
    object_t* o = NULL;
    int32_t scaleFactor = (r->width < r->height) ? r->width : r->height; //Get the smaller of the two
    for(int32_t i = 0; i < r->obj_c; i++) {
        o = r->obj_v + i;
        for(int32_t j = 0; j < o->vert_c; j++) {
            o->proj_v[j].position.x = (((o->vert_v[j].x * r->fov_ratio) / o->vert_v[j].z) * scaleFactor) + ((float) r->width / 2);
            o->proj_v[j].position.y = (((o->vert_v[j].y * r->fov_ratio) / o->vert_v[j].z) * scaleFactor) + ((float) r->height / 2);
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

void determineVisible(renderCtx_t* r) {
    object_t* t = NULL;
    surface_t currentSurf;
    stack_item stItem;
    for(int32_t i = 0; i < r->obj_c; i++) {
        t = r->obj_v + i;
	for(int32_t f = 0; f < t->surface_c; f++){
	    currentSurf = t->surface_v[f];
		
	    //Compute the Z component of the cross product of v1->v3 and v1->v2
		
	    float zComp = ((t->proj_v[currentSurf.v3].position.x - t->proj_v[currentSurf.v1].position.x) //v1->v3's X coord
		* (t->proj_v[currentSurf.v2].position.y - t->proj_v[currentSurf.v1].position.y)) //v1->v2's Y coord
		- ((t->proj_v[currentSurf.v3].position.y - t->proj_v[currentSurf.v1].position.y) //v1->v3's Y coord
		* (t->proj_v[currentSurf.v2].position.x - t->proj_v[currentSurf.v1].position.x)); //v1->v2's X coord
		
		
	    if(zComp < 0) //If Z component of the normal vector is <0, that means the face is pointing towards us
	    {
	        stItem.surf = currentSurf;
	        st_push(&(t->index_st), stItem);
	    }
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
        SDL_RenderGeometry(r->r, NULL, t->proj_v, t->vert_c, (const int*)t->index_st.items, t->index_st.num_items*3);
    }
    SDL_RenderPresent(r->r);
}

void projectObjects(renderCtx_t* r) {
    projectCentral(r);
    determineVisible(r);
    applyColor(r);
    renderScene(r);
}
