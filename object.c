#include "object.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "asset.h"
#include "types.h"


int apply_mat_3x3(matrix_3x3_t m, const vec_3_t* vertices, int32_t num_vertices, vec_3_t* res) {
    for(int32_t i = 0; i < num_vertices; i++) {
        res[i].x = m.column_vectors[0].x * vertices[i].x + m.column_vectors[1].x * vertices[i].y + m.column_vectors[2].x * vertices[i].z;
        res[i].y = m.column_vectors[0].y * vertices[i].x + m.column_vectors[1].y * vertices[i].y + m.column_vectors[2].y * vertices[i].z;
        res[i].z = m.column_vectors[0].z * vertices[i].x + m.column_vectors[1].z * vertices[i].y + m.column_vectors[2].z * vertices[i].z;
    }

    return 0;
}

int apply_vec_3(vec_3_t v, vec_3_t* vertices, int32_t num_vertices, vec_3_t* res) {
    for(int32_t i = 0; i < num_vertices; i++) {
        res[i].x = v.x + vertices[i].x;
        res[i].y = v.y + vertices[i].y;
        res[i].z = v.z + vertices[i].z;
    }

    return 0;
}

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