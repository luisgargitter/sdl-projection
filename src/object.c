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
        malloc(sizeof(*o->vertices_in_scene) * array_length(o->asset->vertices));
    o->projected = array_new(sizeof(SDL_Vertex));
    array_resize(o->projected, array_length(o->asset->vertices));

    o->visible_faces =
        malloc(sizeof(*o->visible_faces) * array_length(o->asset->faces) * 3);
    o->vf_sortable = malloc(sizeof(sortable_triangle) * array_length(o->asset->faces));

    for (int i = 0; i < array_length(o->asset->faces); i++) {
        surface_t tsf = *(surface_t *) array_at(o->asset->faces, i);
        o->vf_sortable[i].p1 = tsf.vertex[0];
        o->vf_sortable[i].p2 = tsf.vertex[1];
        o->vf_sortable[i].p3 = tsf.vertex[2];
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

    free(o->projected);
    o->projected = NULL;

    free(o->visible_faces);
    o->visible_faces = NULL;

    free(o->vf_sortable);
    o->vf_sortable = NULL;
}
