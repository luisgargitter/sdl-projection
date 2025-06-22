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
    o->orientation = orientation;
    o->offset = offset;

    o->vertices_in_scene = array_new(sizeof(vec3_t));
    array_resize(o->vertices_in_scene, array_length(o->asset->vertices));

    o->projected = array_new(sizeof(SDL_Vertex));
    array_resize(o->projected, array_length(o->asset->vertices));

    o->visible_faces = array_new(sizeof(int32_t));
    o->ordered_faces = array_new(sizeof(int32_t));

    return 0;
}

void object_free(object_t *o) {
    if (--o->asset->ref_count <= 0)
        asset_free(o->asset);
    o->asset = NULL;

    free(o->vertices_in_scene);
    o->vertices_in_scene = NULL;

    array_free(o->projected);
    o->projected = NULL;

    array_free(o->visible_faces);
    o->visible_faces = NULL;
}
