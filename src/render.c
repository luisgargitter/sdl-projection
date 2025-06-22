#include "render.h"

// libraries in prelude
#include <SDL2/SDL_stdinc.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "asset.h"
#include "linag.h"
#include "object.h"
#include "projection_error.h"
#include "timsort.h"

#define DEFAULT_CAPACITY_OBJECTS 32

render_t render_init(SDL_Window *w, float_t fov) {
    render_t r;
    r.r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if (r.r == NULL)
        info_and_abort(SDL_GetError());

    SDL_GetWindowSize(w, &r.width, &r.height);

    r.num_objects = 0;
    r.capacity_objects = DEFAULT_CAPACITY_OBJECTS;
    r.objects = malloc(sizeof(object_t) * r.capacity_objects);
    if (r.objects == NULL)
        info_and_abort(NULL);

    r.near = 0.00001;
    r.fov_ratio = tanf(fov / 2.0);
    r.scaled_fov = fminf(r.width, r.height) * r.fov_ratio;

    r.orientation = mat3_rotation(vec3(0, 0, 0));
    r.offset = vec3(0, 0, 0);

    return r;
}

void render_cleanup(render_t p) {
    SDL_DestroyRenderer(p.r);

    for (int32_t i = 0; i < p.num_objects; i++) {
        object_free(p.objects + i);
    }

    p.capacity_objects = 0;
    p.num_objects = 0;
    free(p.objects);
}

int render_add_object(render_t *r, asset_t *asset, mat3_t orientation,
                      vec3_t offset) {
    if (r->num_objects >= r->capacity_objects) {
        r->capacity_objects *= 2;
        r->objects = realloc(r->objects, r->capacity_objects);
    }
    if (r->objects == NULL)
        info_and_abort("NULLPTR");

    object_init(r->objects + r->num_objects, asset, orientation, offset);
    r->num_objects++;

    return 0;
}

void position_object(render_t *r, object_t *o) {
    vec3_t glob_offset = vec3_add(o->offset, r->offset);
    for (int32_t j = 0; j < array_length(o->asset->vertices); j++) {
        vec3_t v = *(vec3_t *) array_at(o->asset->vertices, j);
        v = mat3_mul(o->orientation, v);
        v = vec3_add(glob_offset, v);
        v = mat3_mul(r->orientation, v);
        array_set(o->vertices_in_scene, j, &v);
    }
}

void project_object(render_t *r, object_t *o) {
    vec2_t v2;
    float centerx = (float)r->width / 2;
    float centery = (float)r->height / 2;

    array_clear(o->projected);

    for (int32_t i = 0; i < array_length(o->vertices_in_scene); i++) {
        vec3_t vt = *(vec3_t *) array_at(o->vertices_in_scene, i);
        if (vt.z > 0) {
            v2 = vec2_mul(vec2(vt.x, vt.y), 1/vt.z);
        } else {
            v2 = vec2(0,0);
        }

        SDL_Vertex sv;
        sv.position.x = v2.x * r->scaled_fov + centerx;
        sv.position.y = r->height - (v2.y * r->scaled_fov + centery);

        array_push(o->projected, &sv);
    }
}

void fog_shader(object_t *o) {
    for (int32_t i = 0; i < array_length(o->projected); i++) {
        vec3_t *v = array_at(o->vertices_in_scene, i);
        SDL_Vertex *sv = array_at(o->projected, i);
        SDL_Color *c = &sv->color;
        float distance = vec3_length(*v);
        float lightVal = powf(distance * 10, 1.5);

        c->r = (63 < lightVal) ? 63 : lightVal;
        c->g = (63 < lightVal) ? 63 : lightVal;
        c->b = (63 < lightVal) ? 63 : lightVal;
    }
}

typedef struct {
    float value;
    int arg;
} sortable_t;

int compFaces(const void *f1, const void *f2) {
    sortable_t *st1 = (sortable_t *)f1;
    sortable_t *st2 = (sortable_t *)f2;
    if (st1->value == st2->value)
        return 0;
    if (st1->value < st2->value)
        return 1;
    return -1;
}

void order_faces(render_t *r, object_t *o) {
    array_t *order = array_new(sizeof(sortable_t));

    for (int32_t i = 0; i < array_length(o->visible_faces); i+=3) {
        int32_t *surface = array_at(o->visible_faces, i);
        float distance = 0;
        for (int j = 0; j < 3; j++) {
            vec3_t *v = array_at(o->vertices_in_scene, surface[j]);

            distance = fmaxf(distance, vec3_length(*v));
        }
        sortable_t s = {distance, i};
        array_push(order, &s);
    }
    timsort(array_raw(order), array_length(order), sizeof(sortable_t), compFaces);

    array_clear(o->ordered_faces);
    for (int32_t i = 0; i < array_length(order); i++) {
        sortable_t *sortable = array_at(order, i);
        uint32_t index = sortable->arg;
        array_push(o->ordered_faces, array_at(o->visible_faces, index));
        array_push(o->ordered_faces, array_at(o->visible_faces, index+1));
        array_push(o->ordered_faces, array_at(o->visible_faces, index+2));
    }

    array_free(order);
    order = NULL;
}

bool is_in_front_of_camera(float fnear, object_t *o, surface_t s) {
    vec3_t *v = array_raw(o->vertices_in_scene);
    return (v[s.vertex[0]].z > fnear) && (v[s.vertex[1]].z > fnear) && (v[s.vertex[2]].z > fnear);
}

int frustum_culling(render_t *r, object_t *o) {
    array_truncate(o->vertices_in_scene, array_length(o->asset->vertices));

    array_clear(o->visible_faces);
    for (uint32_t i = 0; i < array_length(o->asset->faces); i+=1) {
        surface_t *surface = array_at(o->asset->faces, i);

        if (is_in_front_of_camera(r->near, o, *surface)) {
            array_push(o->visible_faces, &surface->vertex[0]);
            array_push(o->visible_faces, &surface->vertex[1]);
            array_push(o->visible_faces, &surface->vertex[2]);
        } else {
            int32_t vertices_surface[3];
            vertices_surface[0] = surface->vertex[0];
            vertices_surface[1] = surface->vertex[1];
            vertices_surface[2] = surface->vertex[2];

            for (uint32_t j = 0; j < 3; j++) {
                vec3_t *v1 = array_at(o->vertices_in_scene, vertices_surface[0]);
                vec3_t *v2 = array_at(o->vertices_in_scene, vertices_surface[1]);
                if (v1->z > r->near && v2->z < r->near)
                    break;
                int32_t vi = vertices_surface[0];
                vertices_surface[0] = vertices_surface[2];
                vertices_surface[2] = vertices_surface[1];
                vertices_surface[1] = vi;
            }

            vec3_t vt[3];
            vt[0] = *(vec3_t *) array_at(o->vertices_in_scene, vertices_surface[0]);
            vt[1] = *(vec3_t *) array_at(o->vertices_in_scene, vertices_surface[1]);
            vt[2] = *(vec3_t *) array_at(o->vertices_in_scene, vertices_surface[2]);

            if (vt[0].z > r->near) {
                int32_t visible_surface[3];
                visible_surface[0] = vertices_surface[0];
                visible_surface[1] = array_length(o->vertices_in_scene);
                visible_surface[2] = vertices_surface[2];

                float tt = (r->near - vt[1].z) / (vt[0].z - vt[1].z);
                vec3_t mod_behind = vec3_add(vec3_mul(vt[1], 1-tt), vec3_mul(vt[0], tt));

                array_push(o->vertices_in_scene, &mod_behind);

                if (vt[2].z > r->near) { // surface to quad
                    int32_t additional_surface[3];
                    additional_surface[0] = visible_surface[1];
                    additional_surface[1] = array_length(o->vertices_in_scene);
                    additional_surface[2] = vertices_surface[2];

                    array_push(o->visible_faces, &additional_surface[0]);
                    array_push(o->visible_faces, &additional_surface[1]);
                    array_push(o->visible_faces, &additional_surface[2]);

                    tt = (r->near - vt[1].z) / (vt[2].z - vt[1].z);
                    vec3_t newv = vec3_add(vec3_mul(vt[1], 1-tt), vec3_mul(vt[2], tt));

                    array_push(o->vertices_in_scene, &newv);
                } else {
                    visible_surface[2] = array_length(o->vertices_in_scene);

                    array_push(o->visible_faces, &visible_surface[0]);
                    array_push(o->visible_faces, &visible_surface[1]);
                    array_push(o->visible_faces, &visible_surface[2]);

                    tt = (r->near- vt[2].z) / (vt[0].z - vt[2].z);
                    vec3_t new_other = vec3_add(vec3_mul(vt[2], 1-tt), vec3_mul(vt[0], tt));

                    array_push(o->vertices_in_scene, &new_other);
                }
            }
        }
    }

    return 0;
}

void cull_faces(render_t *r, object_t *o) {
    array_t *visible_faces = array_new(sizeof(int32_t));
    array_resize(visible_faces, array_length(o->visible_faces));

    for (int i = 0; i < array_length(o->visible_faces); i+=3) {
        int32_t *surface = array_at(o->visible_faces, i);
        SDL_Vertex *v1 = array_at(o->projected, surface[0]);
        SDL_Vertex *v2 = array_at(o->projected, surface[1]);
        SDL_Vertex *v3 = array_at(o->projected, surface[2]);
        vec2_t vv1 = vec2(v1->position.x, v1->position.y);
        vec2_t vv2 = vec2(v2->position.x, v2->position.y);
        vec2_t vv3 = vec2(v3->position.x, v3->position.y);

        vec2_t edge1 = vec2_sub(vv2, vv1);
        vec2_t edge2 = vec2_sub(vv3, vv1);
        float z = vec2_cross(edge1, edge2);

        if (z > 0) {
            array_push(visible_faces, surface);
            array_push(visible_faces, surface+1);
            array_push(visible_faces, surface+2);
        }
    }

    array_free(o->visible_faces);
    o->visible_faces = visible_faces;
}

int display_scene(render_t *r) {
    SDL_SetRenderDrawColor(r->r, 0, 0, 0, 255);
    SDL_RenderClear(r->r);

    for (int32_t i = 0; i < r->num_objects; i++) {
        object_t *t = r->objects + i;

        int ret = SDL_RenderGeometry(r->r, NULL, array_raw(t->projected), array_length(t->projected),
                                     array_raw(t->ordered_faces), array_length(t->ordered_faces));
        if(ret < 0) info_and_abort(SDL_GetError());
    }

    return 0;
}

int fadenkreuz(render_t *r) { // does not work
    vec3_t o = {0, 0, 10};
    vec3_t v1 = {1, 0, 0};
    vec3_t v2 = {0, 1, 0};
    vec3_t v3 = {0, 0, 1};
    vec2_t v;
    v1 = vec3_add(o, mat3_mul(r->orientation, v1));
    v = vec2_mul(vec2(v1.x, v1.y), 1/v1.z);
    SDL_SetRenderDrawColor(r->r, 255, 0, 0, 127);
    SDL_RenderDrawLine(r->r, r->width / 2, r->height / 2,
                       v.x * r->scaled_fov + r->width / 2.0,
                       r->height - (v.y * r->scaled_fov + r->height / 2.0));

    v2 = vec3_add(o, mat3_mul(r->orientation, v2));
    v = vec2_mul(vec2(v2.x, v2.y), 1/v2.z);
    SDL_SetRenderDrawColor(r->r, 0, 255, 0, 127);
    SDL_RenderDrawLine(r->r, r->width / 2, r->height / 2,
                       v.x * r->scaled_fov + r->width / 2.0,
                       r->height - (v.y * r->scaled_fov + r->height / 2.0));

    v3 = vec3_add(o, mat3_mul(r->orientation, v3));
    v = vec2_mul(vec2(v3.x, v3.y), 1/v3.z);
    SDL_SetRenderDrawColor(r->r, 0, 0, 255, 127);
    SDL_RenderDrawLine(r->r, r->width / 2, r->height / 2,
                       v.x * r->scaled_fov + r->width / 2.0,
                       r->height - (v.y * r->scaled_fov + r->height / 2.0));

    return 0;
}

void render_objects(render_t *r) {
    for (int32_t i = 0; i < r->num_objects; i++) {
        object_t *o = r->objects + i;
        position_object(r, o);
        frustum_culling(r, o);
        order_faces(r, o);
        project_object(r, o);
        cull_faces(r, o);
        fog_shader(o);
    }
}

int render_frame(render_t *r) {
    render_objects(r);
    display_scene(r);

    fadenkreuz(r);

    SDL_RenderPresent(r->r);
    return 0;
}
