#include "cube.h"
#include "types.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void genVerts(float edgelen, vec_3_t vert_v[8]) {
    int32_t ind = 0;
    for(int32_t i = 0; i < 2; i++) {
        for(int32_t j = 0; j < 2; j++) {
            for(int32_t k = 0; k < 2; k++) {
                vert_v[ind].x = i * edgelen;
                vert_v[ind].y = j * edgelen;
                vert_v[ind].z = k * edgelen;
                ind++;
            }
        }
    }
}

void connectToTriangles(int32_t* index_v) {
    int32_t edgeLUT[3 * 12] = {
        0, 1, 2,    0, 2, 4,    0, 4, 1,
        3, 1, 2,    3, 2, 7,    3, 7, 1,
        5, 1, 4,    5, 4, 7,    5, 7, 1,
        6, 2, 4,    6, 4, 7,    6, 7, 2
    };

    for(int32_t i = 0; i < 3 * 12; i++) {
        index_v[i] = edgeLUT[i];
    }
}

object_t* cubeNew(float edgelen) {
    if(edgelen < 0) return NULL;
    object_t* o = malloc(sizeof(object_t));
    if(o == NULL) return NULL;
    o->vert_c = 8;
    o->vert_v = malloc(sizeof(*(o->vert_v)) * o->vert_c);
    if(o->vert_v == NULL) return NULL;
    o->proj_v = malloc(sizeof(*(o->proj_v)) * o->vert_c);
    if(o->proj_v == NULL) return NULL;
    o->index_c = 3 * 12;
    o->index_v = malloc(sizeof(*(o->index_v)) * o->index_c);
    if(o->index_v == NULL) return NULL;

    genVerts(edgelen, o->vert_v);
    
    connectToTriangles(o->index_v);
    
    return o;
}

void cubeFree(object_t* cube) {
    free(cube->index_v);
    free(cube->vert_v);
    free(cube);
}

void objectMove(object_t* obj, vec_3_t off) {
    for(int32_t i = 0; i < obj->vert_c; i++) {
        obj->vert_v[i].x += off.x;
        obj->vert_v[i].y += off.y;
        obj->vert_v[i].z += off.z;
    }
}

