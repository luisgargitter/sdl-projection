#include "render.h"

// libraries in prelude
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "asset.h"
#include "linag.h"
#include "object.h"
#include "projection_error.h"
#include "timsort.h"

#define DEFAULT_CAPACITY_OBJECTS 32

int render_init(render_t* r, SDL_Window *w, float_t fov) {
    r->r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if(r->r == NULL) info_and_abort(SDL_GetError());

    SDL_GetWindowSize(w, &r->width, &r->height);

    r->num_objects = 0;
    r->capacity_objects = DEFAULT_CAPACITY_OBJECTS;
    r->objects = malloc(sizeof(object_t) * r->capacity_objects);
    if(r->objects == NULL) info_and_abort(NULL);
    
    r->fov_ratio = tanf(fov / 2.0);
    r->scaled_fov = fminf(r->width, r->height) * r->fov_ratio;

    r->orientation = matrix_3x3_rotation(vec_3(0, 0, 0));
    r->offset = vec_3(0, 0, 0);
    
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
    if(r->objects == NULL) info_and_abort("NULLPTR");

    object_init(r->objects + r->num_objects, asset, orientation, offset);
    r->num_objects++;

    return 0;
}

int render_position(render_t* r) {
    for(int32_t i = 0; i < r->num_objects; i++) {
        object_position(
            r->objects + i,
            r->objects[i].orientation, 
            r->objects[i].offset
        );
        apply_vec_3(
            r->offset,
            r->objects[i].vertices_in_scene, 
            r->objects[i].asset->v_count, 
            r->objects[i].vertices_in_scene
        );
        apply_mat_3x3(r->orientation, r->objects[i].vertices_in_scene, r->objects[i].asset->v_count, r->objects[i].vertices_in_scene);
    }

    return 0;
}

void projectCentral(render_t* r) {
    float centerx = (float) r->width / 2;
    float centery = (float) r->height / 2;

    for(int32_t i = 0; i < r->num_objects; i++) {
        object_t* o = r->objects + i;
        vec_3_t* v = o->vertices_in_scene;

        for(int32_t j = 0; j < o->asset->v_count; j++) {
            vec_2_t v2 = vec_3_map_to_plane(v[j]);
            o->proj_v[j].position.x = v2.x * r->scaled_fov + centerx;
            o->proj_v[j].position.y = r->height - (v2.y * r->scaled_fov + centery);

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
    
    for (int32_t j = 0; j < r->num_objects; j++) {
      object_t *o = r->objects + j;
      vec_3_t* v = o->vertices_in_scene;
      for (int32_t k = 0; k < o->asset->v_count; k++) {
        float lightVal = (10.0 / powf(vec_3_euclidean_distance(vec_3_identity(), v[k]),2));
        o->proj_v[k].color.r = (255 < lightVal) ? 255 : lightVal;
        o->proj_v[k].color.g = (255 < lightVal) ? 255 : lightVal;
        o->proj_v[k].color.b = (255 < lightVal) ? 255 : lightVal;
        o->proj_v[k].color.r = rand();
        o->proj_v[k].color.g = rand();
        o->proj_v[k].color.b = rand();
        o->proj_v[k].color.a = 255;
      }
    }
}

int compFaces(const void* f1, const void* f2) {
    sortable_triangle* st1 = (sortable_triangle*)f1;
    sortable_triangle* st2 = (sortable_triangle*)f2;
    if (st1->farthest == st2->farthest)
        return 0;
    if (st1->farthest < st2->farthest)
        return 1;
    return -1;
}

void sortFaces(object_t *obj) {
    timsort(obj->vf_sortable, obj->asset->f_count, sizeof(sortable_triangle), compFaces);
}

bool is_in_front_of_camera(object_t* o, sortable_triangle s) {
    vec_3_t* v =  o->vertices_in_scene;
    if(v[s.p1].z > 0) return true;
    if(v[s.p2].z > 0) return true;
    if(v[s.p3].z > 0) return true;
    return false;
}

void determineVisible(render_t* r) { // no touching! (holy grale of projection, boris allowed only)
    for(int32_t i = 0; i < r->num_objects; i++) {
        object_t* t = r->objects + i;
        t->vf_count = 0;

        for(int32_t f = 0; f < t->asset->f_count; f++){
            sortable_triangle* st = &(t->vf_sortable[f]);
            vec_3_t *v = t->vertices_in_scene;
            float_t dist1 = leud(v[st->p1], vec_3_identity());
            float_t dist2 = leud(v[st->p2], vec_3_identity());
            float_t dist3 = leud(v[st->p3], vec_3_identity());
            float_t max = fmaxf(dist1, dist2);
            max = fmaxf(max, dist3);
            st->farthest = max;
        }
        sortFaces(t);
        for(int32_t f = 0; f < t->asset->f_count; f++){
            surface_t* currentSurf = t->asset->f_vector + f;
            sortable_triangle st = t->vf_sortable[f];

	        //Compute the Z component of the cross product of v1->v3 and v1->v2
	        float zComp = ((t->proj_v[st.p3].position.x - t->proj_v[st.p1].position.x) //v1->v3's X coord
		    * (t->proj_v[st.p2].position.y - t->proj_v[st.p1].position.y)) //v1->v2's Y coord
		    - ((t->proj_v[st.p3].position.y - t->proj_v[st.p1].position.y) //v1->v3's Y coord
		    * (t->proj_v[st.p2].position.x - t->proj_v[st.p1].position.x)); //v1->v2's X coord
		
	        if(zComp < 0 && is_in_front_of_camera(t, st)) //If Z component of the normal vector is <0, that means the face is pointing towards us
	        {  
                //So we add the current face to the array of visible, in the format that SDL wants
                int32_t* temp = t->visible_faces + t->vf_count * 3;
                temp[0] = t->vf_sortable[f].p1;
                temp[1] = t->vf_sortable[f].p2;
                temp[2] = t->vf_sortable[f].p3;
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

        //printf("Drawing %d/%d faces over %d vertices\n", t->vf_count, t->asset->f_count, t->asset->v_count);
        
        int ret = SDL_RenderGeometry(r->r, NULL, 
            t->proj_v, t->asset->v_count, t->visible_faces, t->vf_count * 3);
        //if(ret < 0) info_and_abort(SDL_GetError());
    }

    return 0;
}

int fadenkreuz(render_t* r) { // does not work
    vec_3_t o = {0, 0, 10}; 
    vec_3_t v1 = {1, 0, 0};
    vec_3_t v2 = {0, 1, 0};
    vec_3_t v3 = {0, 0, 1};
    vec_2_t v;
    v = vec_3_map_to_plane(ladd(o, lmul(r->orientation, v1)));
    SDL_SetRenderDrawColor(r->r, 255, 0, 0, 255);
    SDL_RenderDrawLine(r->r, r->width/2, r->height/2, 
        v.x  * r->scaled_fov + r->width / 2.0, 
        r->height - (v.y  * r->scaled_fov + r->height / 2.0)
    );
    
    v = vec_3_map_to_plane(ladd(o, lmul(r->orientation, v2)));
    SDL_SetRenderDrawColor(r->r, 0, 255, 0, 255);    
    SDL_RenderDrawLine(r->r, r->width/2, r->height/2, 
        v.x  * r->scaled_fov + r->width / 2.0, 
        r->height - (v.y  * r->scaled_fov + r->height / 2.0)
    );
    
v = vec_3_map_to_plane(ladd(o, lmul(r->orientation, v3)));
    SDL_SetRenderDrawColor(r->r, 0, 0, 255, 255);
    SDL_RenderDrawLine(r->r, r->width/2, r->height/2, 
        v.x  * r->scaled_fov + r->width / 2.0, 
        r->height - (v.y  * r->scaled_fov + r->height / 2.0)
    );
   
    return 0; 
}


int projectObjects(render_t* r) {
    render_position(r);
    projectCentral(r);
    determineVisible(r);
    applyColor(r);
    int r1 = renderScene(r);

    fadenkreuz(r);

    SDL_RenderPresent(r->r);
    return r1;
}
