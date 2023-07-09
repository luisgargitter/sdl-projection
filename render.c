#include "render.h"
#include "types.h"
#include "object.h"
#include "asset.h"


#include <SDL2/SDL_error.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>

renderCtx_t* renderCtxNew(SDL_Window *w, int32_t obj_c, float_t fov) {
    if(obj_c <= 0) return NULL;

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
    free(p->obj_v);
    p->obj_v = NULL;
    p->obj_c = 0;
    free(p);
}

void projectCentral(renderCtx_t* r) {
    float scaleFactor = fminf(r->width, r->height); // TODO: should only be calculated on WindowEvent<Resize>
    float centerx = (float) r->width / 2;
    float centery = (float) r->height / 2;
    float scaled_fov = scaleFactor * r->fov_ratio;

    for(int32_t i = 0; i < r->obj_c; i++) {
        object_t* o = r->obj_v + i;
        vec_3_t* v = o->asset->v_vector;
        vec_3_t offset = o->scene_offset;

        for(int32_t j = 0; j < o->asset->v_count; j++) {
            o->proj_v[j].position.x = ((v[j].x + offset.x) / (v[j].z + offset.z)) * scaled_fov + centerx;
            o->proj_v[j].position.y = r->height - (((v[j].y + offset.y) / (v[j].z + offset.z)) * scaled_fov + centery);
        }
    }
}

void applyColor(renderCtx_t* r) { // temp function for testing. will be replaced by shading function in future
    uint8_t colors[3] = {0, 127, 255};
    srand((unsigned int) time(NULL));

    for(int32_t j = 0; j < r->obj_c; j++) {
        object_t* o = r->obj_v + j;
        for(int32_t k = 0; k < o->asset->v_count; k++) {
            o->proj_v[k].color.r = colors[rand()%3];
            o->proj_v[k].color.g = colors[rand()%3];
            o->proj_v[k].color.b = colors[rand()%3];
            o->proj_v[k].color.a = 255;
        }
    }
}

void determineVisible(renderCtx_t* r) {
    for(int32_t i = 0; i < r->obj_c; i++) {
        object_t* t = r->obj_v + i;
        t->vf_count = 0;

        for(int32_t f = 0; f < t->asset->f_count; f++){
            int32_t* currentSurf = t->asset->f_vector + f * 3;

	        //Compute the Z component of the cross product of v1->v3 and v1->v2
		
	        float zComp = ((t->proj_v[currentSurf[2]].position.x - t->proj_v[currentSurf[0]].position.x) //v1->v3's X coord
		    * (t->proj_v[currentSurf[1]].position.y - t->proj_v[currentSurf[0]].position.y)) //v1->v2's Y coord
		    - ((t->proj_v[currentSurf[2]].position.y - t->proj_v[currentSurf[0]].position.y) //v1->v3's Y coord
		    * (t->proj_v[currentSurf[1]].position.x - t->proj_v[currentSurf[0]].position.x)); //v1->v2's X coord
		
		
	        if(zComp < 0) //If Z component of the normal vector is <0, that means the face is pointing towards us
	        {
	            int32_t* temp = t->visible_faces + t->vf_count * 3;
                temp[0] = currentSurf[0];
                temp[1] = currentSurf[1];
                temp[2] = currentSurf[2];
                t->vf_count++;
	        }
	    }
    }
}

int renderScene(renderCtx_t* r) {
    SDL_SetRenderDrawColor(r->r, 0, 0, 0, 255);
    SDL_RenderClear(r->r);
    //SDL_SetRenderDrawColor(r->r, 255, 255, 255, 255);
    
    for(int32_t i = 0; i < r->obj_c; i++) {
        object_t* t = r->obj_v + i;

        int ret = SDL_RenderGeometry(r->r, NULL, 
            t->proj_v, t->asset->v_count, t->visible_faces, t->vf_count * 3);
        if(ret < 0) return ret;
    }
    SDL_RenderPresent(r->r);

    return 0;
}

int projectObjects(renderCtx_t* r) {
    projectCentral(r);
    determineVisible(r);
    applyColor(r);
    if(renderScene(r) == -1) return -1;

    return 0;
}
