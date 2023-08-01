#include "object.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "asset.h"

int update_position(object_t *o, matrix_3x3_t orientation, vec_3_t offset) {
    apply_mat_3x3(orientation, o->asset->v_vector, o->asset->v_count, o->vertices_in_scene);
    apply_vec_3(offset, o->vertices_in_scene, o->asset->v_count, o->vertices_in_scene);

    return 0;
}

int object_init(object_t* o, asset_t* a, matrix_3x3_t orientation, vec_3_t offset) {
    a->ref_count++; // increasing reference count for automatic deallocation, when no longer in use
    o->asset = a;
    o->asset->ref_count++;
    o->vf_count = 0;

    o->vertices_in_scene = malloc(sizeof(*o->vertices_in_scene) * o->asset->v_count);
    o->proj_v = malloc(sizeof(*o->proj_v) * o->asset->v_count);
    o->visible_faces = malloc(sizeof(*o->visible_faces) * o->asset->f_count * 3);

    update_position(o, orientation, offset);

    return 0;
}

void object_free(object_t* o) {
    if(--o->asset->ref_count <= 0) asset_free(o->asset);
    o->asset = NULL;

    free(o->vertices_in_scene);
    o->vertices_in_scene = NULL;
    o->vf_count = 0;

    free(o->proj_v);
    o->proj_v = NULL;

    free(o->visible_faces);
    o->visible_faces = NULL;
}