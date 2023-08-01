#include "render.h"
#include "object.h"
#include "asset.h"
#include "projection_error.h"


#include <SDL2/SDL_error.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>

#define DEFAULT_CAPACITY_OBJECTS 32

int render_init(render_t* r, SDL_Window *w, float_t fov) {
    r->r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if(r->r == NULL) return -2;

    SDL_GetWindowSize(w, &r->width, &r->height);

    r->num_objects = 0;
    r->capacity_objects = DEFAULT_CAPACITY_OBJECTS;
    r->objects = malloc(sizeof(object_t) * r->capacity_objects);
    if(r->objects == NULL) info_and_abort(NULL);
    
    r->fov_ratio = tanf(fov / 2.0);

    r->orientation = matrix_3x3_identity();
    r->offset.x = 0;
    r->offset.y = 0;
    r->offset.z = 0;
    
    return 0;
}

void render_free(render_t* p) {
    SDL_DestroyRenderer(p->r);

    for(int32_t i = 0; i < p->num_objects; i++) {
        object_free(p->objects + i);
    }

    p->capacity_objects = 0;
    p->num_objects = 0;
    free(p->objects);
}

int render_add_object(render_t* r, asset_t* asset, matrix_3x3_t orientation, vec_3_t offset) {
    if(r->num_objects >= r->capacity_objects) {
        r->capacity_objects *= 2;
        r->objects = realloc(r->objects, r->capacity_objects);
    }

    object_init(r->objects + r->num_objects, asset, orientation, offset);
    r->num_objects++;

    return 0;
}

int update_offset(render_t* r) {
    for(int32_t i = 0; i < r->num_objects; i++) {
        apply_vec_3(r->offset, r->objects[i].vertices_in_scene, r->objects[i].asset->v_count, r->objects[i].vertices_in_scene);
    }

    return 0;
}

void projectCentral(render_t* r) {
    float scaleFactor = fminf(r->width, r->height); // TODO: should only be calculated on WindowEvent<Resize>
    float centerx = (float) r->width / 2;
    float centery = (float) r->height / 2;
    float scaled_fov = scaleFactor * r->fov_ratio;

    for(int32_t i = 0; i < r->num_objects; i++) {
        object_t* o = r->objects + i;
        vec_3_t* v = o->vertices_in_scene;

        for(int32_t j = 0; j < o->asset->v_count; j++) {
            o->proj_v[j].position.x = (v[j].x / v[j].z) * scaled_fov + centerx;
            o->proj_v[j].position.y = r->height - ((v[j].y / v[j].z) * scaled_fov + centery);

            //printf("%f %f\n", o->proj_v[j].position.x, o->proj_v[j].position.y);
        }
    }
}

void applyColor(render_t* r) { // temp function for testing. will be replaced by shading function in future
    // execute once
    static int i = 0;
    if(i == 1) return;
    i = 1;

    uint8_t colors[3] = {0, 127, 255};
    srand((unsigned int) time(NULL));

    for(int32_t j = 0; j < r->num_objects; j++) {
        object_t* o = r->objects + j;
        for(int32_t k = 0; k < o->asset->v_count; k++) {
            o->proj_v[k].color.r = colors[rand()%3];
            o->proj_v[k].color.g = colors[rand()%3];
            o->proj_v[k].color.b = colors[rand()%3];
            o->proj_v[k].color.a = 255;
        }
    }
}

void determineVisible(render_t* r) {
    for(int32_t i = 0; i < r->num_objects; i++) {
        object_t* t = r->objects + i;
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

int renderScene(render_t* r) {
    SDL_SetRenderDrawColor(r->r, 0, 0, 0, 255);
    SDL_RenderClear(r->r);
    //SDL_SetRenderDrawColor(r->r, 255, 255, 255, 255);
    
    for(int32_t i = 0; i < r->num_objects; i++) { 
        object_t* t = r->objects + i;

        // printf("Drawing %d/%d faces over %d vertices\n", t->vf_count, t->asset->f_count, t->asset->v_count);
        int ret = SDL_RenderGeometry(r->r, NULL, 
            t->proj_v, t->asset->v_count, t->visible_faces, t->vf_count * 3);
        if(ret < 0) return ret;
    }
    SDL_RenderPresent(r->r);

    return 0;
}

int projectObjects(render_t* r) {
    update_offset(r);
    projectCentral(r);
    determineVisible(r);
    applyColor(r);
    int r1 = renderScene(r);
    
    return r1;
}
