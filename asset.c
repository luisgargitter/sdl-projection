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
#include "projection_error.h"

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

int32_t obj_scan_surface(surface_t* s, char* string) { // only works for triangles, not for all polygons
    //printf("hello world\n");
    int f[9];
    int32_t matched = sscanf(
        string, "%d%*c%d%*c%d %d%*c%d%*c%d %d%*c%d%*c%d", 
        f+0, f+1, f+2,
        f+3, f+4, f+5,
        f+6, f+7, f+8
    );
    //printf("matched: %d\n", matched);
    int vo = 1;
    if(matched == 6) vo = 2;
    else if(matched == 9) vo = 3;
      
    s->vertex[0] = f[0 * vo];
    s->vertex[1] = f[1 * vo];
    s->vertex[2] = f[2 * vo];

    return 0;
}

int obj_proc_line(asset_t* asset, char* line) {
    // meant only for testing, works with teapot.obj
    if(line[0] == 'v') {
        if(line[1] == ' ') {
            obj_scan_vec_3(asset->v_vector + asset->v_count, line + 1);
            asset->v_count++;
        } else if(line[1] == 'n') {
            //obj_scan_vec_3(asset->n_vector + asset->n_count, line + 2);
            //asset->n_count++;
        } else if(line[1] == 't') {
            //obj_scan_vec_2(asset->t_vector + asset->t_count, line + 2);
            //asset->t_count++;
        }
    } else if(*line == 'f') {
        surface_t* sf = asset->f_vector + asset->f_count;
        obj_scan_surface(sf, line + 1);

        // indexing in obj files starts with 1 not 0.
        sf->vertex[0]--;
        sf->vertex[1]--;
        sf->vertex[2]--;

        asset->f_count++;
    }

    return 0;
}

int asset_load_obj(FILE* f, asset_t* a) {
    int32_t vc = DEFAULT_VERTEX_COUNT;
    int32_t nc = DEFAULT_NORMAL_COUNT;
    int32_t tc = DEFAULT_TEXTURE_COUNT;
    int32_t fc = DEFAULT_FACE_COUNT;

    bool contains_vertex_normals = false;
    bool contains_texture_coordinates = false;
    a->v_vector = malloc(sizeof(*(a->v_vector)) * vc);
    if(a->v_vector == NULL) info_and_abort("NULLPTR");
    a->v_count = 0;
    
    a->n_vector = malloc(sizeof(*(a->n_vector)) * nc);
    if(a->n_vector == NULL) info_and_abort("NULLPTR");
    a->n_count = 0;

    a->t_vector = malloc(sizeof(*(a->t_vector)) * tc);
    if(a->t_vector == NULL) info_and_abort("NULLPTR");
    a->t_count = 0;

    a->f_vector = malloc(sizeof(*(a->f_vector)) * fc);
    if(a->f_vector == NULL) info_and_abort("NULLPTR");
    a->f_count = 0;

    char* line = NULL;
    size_t n = 0;
    
    while(getline(&line, &n, f) > 0) {
        obj_proc_line(a, line);
        if(a->v_count >= vc) {
            vc = vc * 2;
            a->v_vector = realloc(a->v_vector, sizeof(*(a->v_vector)) * vc);
        } else if(a->n_count >= nc) {
            nc = nc * 2;
            a->n_vector = realloc(a->n_vector, sizeof(*(a->n_vector)) * nc);
        } else if(a->t_count >= tc) {
            tc = tc * 2;
            a->t_vector = realloc(a->t_vector, sizeof(*(a->t_vector)) * tc);
        } else if(a->f_count >= fc) {
            fc = fc * 2;
            a->f_vector = realloc(a->f_vector, sizeof(*(a->f_vector)) * fc);
        }

        free(line);
        line = NULL;
    }

    // filtering optional data.
    if(contains_vertex_normals == false) {
        free(a->n_vector);
        a->n_vector = NULL;
        nc = 0;
    }
    if(contains_texture_coordinates == false) {
        free(a->t_vector);
        a->t_vector = NULL;
        tc = 0;
    }

    return 0;
}


void asset_free(asset_t* a) {
    a->v_count = 0;
    free(a->v_vector);
    free(a->n_vector);
    free(a->t_vector);
    a->f_count = 0;
    free(a->f_vector);
}
