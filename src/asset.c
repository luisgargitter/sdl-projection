//
// Created by luis on 06.07.23.
//

#include "asset.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linag.h"
#include "projection_error.h"
#include "array.h"

#define DEFAULT_VERTEX_COUNT 512
#define DEFAULT_NORMAL_COUNT 512
#define DEFAULT_TEXTURE_COUNT 512
#define DEFAULT_FACE_COUNT 512

// .obj file parsing

int32_t obj_scan_vec_3(vec3_t *v, char *string) {
    // expection a string containing 3 floating point numbers
    if (sscanf(string, "%f %f %f", &v->x, &v->y, &v->z) != 3)
        return -1;

    return 0;
}

int32_t obj_scan_vec_2(vec2_t *v, char *string) {
    // expecting a string containing 3 floating point numbers
    if (sscanf(string, "%f %f", &v->x, &v->y) != 2)
        return -1;

    return 0;
}

// only works for triangles, not for all polygons
int32_t obj_scan_surface(surface_t *s, char *string) {
    int f[9];
    int32_t matched =
        sscanf(string, "%d%*c%d%*c%d %d%*c%d%*c%d %d%*c%d%*c%d",
            f + 0, f + 1, f + 2, f + 3, f + 4, f + 5, f + 6, f + 7, f + 8);

    int vo = 1;
    if (matched == 6)
        vo = 2;
    else if (matched == 9)
        vo = 3;

    s->vertex[0] = f[0 * vo];
    s->vertex[1] = f[1 * vo];
    s->vertex[2] = f[2 * vo];

    return 0;
}

int obj_proc_line(asset_t *asset, char *line) {
    // meant only for testing, works with teapot.obj
    vec3_t v3;
    vec2_t v2;
    surface_t s;

    switch (line[0]) {
    case 'v':
        switch (line[1]) {
        case ' ':
            if (obj_scan_vec_3(&v3, line + 1) == -1)
                return -1;

            array_push(asset->vertices, &v3);
            break;
        case 'n':
            /*
            if (obj_scan_vec_3(&v3, line + 1) == -1)
                return -1;

            array_push(asset->normals, &v3);
            */
            break;

        case 't':
            /*
            if (obj_scan_vec_2(&v2, line + 1) == -1)
                return -1;

            array_push(asset->uvcoordinates, &v2);
            */
            break;

        default:
            return -1;
        }
        break;
    case 'f':
        if (obj_scan_surface(&s, line + 1) == -1)
            return -1;

        // indexing in obj files starts with 1 not 0.
        s.vertex[0]--;
        s.vertex[1]--;
        s.vertex[2]--;

        array_push(asset->faces, &s);
        break;
    case 's':
        // whatever that does
        break;
    case 'u':
        // allow usemtl (use material)
        break;
    case '#':
        // allow comments
        break;
    case '\n':
        // allow empty lines
        break;
    default:
        return -1; // invalid format
    }

    return 0;
}

void asset_load_obj(FILE *f, asset_t *a) {
    a->vertices = array_new(sizeof(vec3_t));
    a->normals = array_new(sizeof(vec3_t));
    a->uv_coordinates = array_new(sizeof(vec2_t));
    a->faces = array_new(sizeof(surface_t));

    bool contains_vertex_normals = false;
    bool contains_texture_coordinates = false;

    char *line = NULL;
    size_t n = 0;

    while (getline(&line, &n, f) > 0) {
        if (obj_proc_line(a, line) == -1) {
            info_and_abort("Format error\n");
        }
    }

    // filtering optional data.
    if (contains_vertex_normals == false) {
        array_free(a->normals);
        a->normals = NULL;
    }
    if (contains_texture_coordinates == false) {
        array_free(a->uv_coordinates);
        a->uv_coordinates = NULL;
    }
}

void asset_free(asset_t *a) {
    array_free(a->vertices);
    a->vertices = NULL;
    array_free(a->normals);
    a->normals = NULL;
    array_free(a->uv_coordinates);
    a->uv_coordinates = NULL;
    array_free(a->faces);
    a->faces = NULL;
}
