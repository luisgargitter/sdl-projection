//
// Created by luis on 06.07.23.
//

#include "asset.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "linag.h"

#define DEFAULT_VERTEX_COUNT 512
#define DEFAULT_NORMAL_COUNT 512
#define DEFAULT_TEXTURE_COUNT 512
#define DEFAULT_FACE_COUNT 512

// .obj file parsing

int32_t obj_scan_vec_3(vec_3_t* v, char* string) {
    // expection a string containing 3 floating point numbers
    int32_t matched = sscanf(string, "%f %f %f", &v->x, &v->y, &v->z);
    
    return matched == 3 ? 0 : -1;
}

int32_t obj_scan_vec_2(vec_2_t* v, char* string) {
    // expecting a string containing 3 floating point numbers
    int32_t matched = sscanf(string, "%f %f", &v->x, &v->y);
    
    return matched == 2 ? 0 : -1;
}

int32_t obj_scan_surface(surface_t* s, char* string) { // unfinished
    int32_t matched = sscanf(
        string, "%d/%d/%d %d/%d/%d %d/%d/%d", 
        s->vertex+0, s->texture+0, s->normal+0, 
        s->vertex+1, s->texture+1, s->normal+1, 
        s->vertex+2, s->texture+2, s->normal+2
    );
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
    int32_t vc = DEFAULT_VERTEX_COUNT;
    int32_t nc = DEFAULT_NORMAL_COUNT;
    int32_t vtc = DEFAULT_TEXTURE_COUNT;
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
