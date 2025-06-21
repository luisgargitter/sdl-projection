#include "object.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "asset.h"
#include "linag.h"

int object_init(object_t *o, asset_t *a, mat3_t orientation, vec3_t offset) {
    a->ref_count++; // increasing reference count for automatic deallocation,
                    // when no longer in use
    o->asset = a;
    o->asset->ref_count++;
    o->vf_count = 0;
    o->orientation = orientation;
    o->offset = offset;

    o->vertices_in_scene =
        malloc(sizeof(*o->vertices_in_scene) * o->asset->v_count);
    o->proj_v = malloc(sizeof(*o->proj_v) * o->asset->v_count);
    o->visible_faces =
        malloc(sizeof(*o->visible_faces) * o->asset->f_count * 3);
    o->vf_sortable = malloc(sizeof(sortable_triangle) * o->asset->f_count);

    for (int i = 0; i < o->asset->f_count; i++) {
        o->vf_sortable[i].p1 = o->asset->f_vector[i].vertex[0];
        o->vf_sortable[i].p2 = o->asset->f_vector[i].vertex[1];
        o->vf_sortable[i].p3 = o->asset->f_vector[i].vertex[2];
    }

    return 0;
}

void object_free(object_t *o) {
    if (--o->asset->ref_count <= 0)
        asset_free(o->asset);
    o->asset = NULL;

    free(o->vertices_in_scene);
    o->vertices_in_scene = NULL;
    o->vf_count = 0;

    free(o->proj_v);
    o->proj_v = NULL;

    free(o->visible_faces);
    o->visible_faces = NULL;

    free(o->vf_sortable);
    o->vf_sortable = NULL;
}
