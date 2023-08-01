//
// Created by luis on 06.07.23.
//

#include "asset.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "linag.h"

#define DEFAULT_VERT_COUNT 512
#define DEFAULT_FACE_COUNT 512

int resize_asset(asset_t* a, int32_t vertex_count, int32_t face_count) {
    //a->vn_vector = realloc(a->vn_vector, sizeof(*a->vn_vector) * vertex_count);
    //a->vt_vector = realloc(a->vt_vector, sizeof(*a->vt_vector) * vertex_count);

    return 0;
}

int obj_proc_line(asset_t* asset, char* line) {
    // meant only for testing, works with teapot.obj
    if(*line == 'v') {
        vec_3_t* t = asset->v_vector + asset->v_count;
        sscanf(line, "v %f %f %f", &t->x, &t->y, &t->z);

        asset->v_count++;
    } else if(*line == 'f') {
        int32_t* triplet = asset->f_vector + asset->f_count * 3;
        sscanf(line, "f %d %d %d", triplet + 0, triplet + 1, triplet + 2);

        // indexing in obj files starts with 1 not 0.
        triplet[0]--;
        triplet[1]--;
        triplet[2]--;

        asset->f_count++;
    }

    return 0;
}

int asset_load_obj(FILE* f, asset_t* a) {
    int32_t vc = DEFAULT_VERT_COUNT;
    int32_t fc = DEFAULT_FACE_COUNT;

    bool contains_vertex_normals = false;
    bool contains_texture_coordinates = false;
    a->v_vector = malloc(sizeof(*(a->v_vector)) * vc);
    if(a->v_vector == NULL) return -1;
    a->v_count = 0;
    a->f_vector = malloc(sizeof(*(a->f_vector)) * fc * 3);
    if(a->f_vector == NULL) return -1;
    a->f_count = 0;

    a->vn_vector = NULL;
    a->vt_vector = NULL;

    //resize_asset(a, vc, fc);


    char* line = NULL;
    size_t n = 0;
    
    while(getline(&line, &n, f) > 0) {
        obj_proc_line(a, line);
        if(a->v_count >= vc) {
            vc = vc * 2;
            a->v_vector = realloc(a->v_vector, sizeof(*(a->v_vector)) * vc);
        } else if(a->f_count >= fc) {
            fc = fc * 2;
            a->f_vector = realloc(a->f_vector, sizeof(*(a->f_vector)) * fc * 3);
        }

        free(line);
        line = NULL;
    }

    // filtering optional data.
    if(contains_vertex_normals == false) {
        free(a->vn_vector);
        a->vn_vector = NULL;
    }
    if(contains_texture_coordinates == false) {
        free(a->vt_vector);
        a->vt_vector = NULL;
    }

    return 0;
}


void asset_free(asset_t* a) {
    a->v_count = 0;
    free(a->f_vector);
    free(a->vn_vector);
    free(a->vt_vector);
    a->f_count = 0;
    free(a->f_vector);
}