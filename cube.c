#include "cube.h"
#include "types.h"
#include "test.h"

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

void connectToTriangles(object_t* obj) {
    int32_t surfaceLUT[12][3] = {
        {0, 2, 1},    {1, 2, 3},
        {0, 4, 2},    {6, 2, 4},
        {4, 0, 1},	  {4, 1, 5},
        {3, 2, 7},    {6, 7, 2},
        {3, 7, 1},    {5, 1, 7},
        {5, 7, 4},    {6, 4, 7}
    };

    for(int32_t i = 0; i < obj->surface_c; i++) {
        obj->surface_v[i].v1 = surfaceLUT[i][0];
        obj->surface_v[i].v2 = surfaceLUT[i][1];
        obj->surface_v[i].v3 = surfaceLUT[i][2];
    }
}

Error_t cubeNew(float edgelen, object_t* o) {
    if(edgelen < 0) return ERR_NULLPTR;
    if(o == NULL) return ERR_NULLPTR;
    o->vert_c = 8;
    o->vert_v = malloc(sizeof(*(o->vert_v)) * o->vert_c);
    if(o->vert_v == NULL) return ERR_NULLPTR;
    o->proj_v = malloc(sizeof(*(o->proj_v)) * o->vert_c);
    if(o->proj_v == NULL) return ERR_NULLPTR;
    
    o->surface_c = 12;
    o->surface_v = malloc(sizeof(*(o->surface_v)) * o->surface_c);
    if(o->surface_v == NULL) return ERR_NULLPTR;
    
    o->index_st = st_create(6); //A cube has no more than 6 triangles visible at a time

    genVerts(edgelen, o->vert_v);
    
    connectToTriangles(o);
    
    return ERR_OK;
}

void cubeFree(object_t* cube) {
    free(cube->surface_v);
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


/* ------==================------ */
/* ------====== TEST ======------ */
/* ------==================------ */

#ifdef CTEST

/**
* @brief Test the cubeNew Interface
*/
void test_cubeNew()
{
    object_t obj;

    /* test if a call with edglen smaller than zero triggers an error */
    CU_ASSERT_EQUAL(cubeNew(-1.0f, &obj), ERR_NULLPTR);

    /* test if a null object triggers an error */
    CU_ASSERT_EQUAL(cubeNew(3.0f, NULL), ERR_NULLPTR);
}
#endif
