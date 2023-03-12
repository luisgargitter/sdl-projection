#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

#include "eventhandler.h"
#include "test.h"

#include "types.h"
#include "render.h"
#include "cube.h"

#define WIDTH 800
#define HEIGHT 600

#define CUBEVERTC 8 * DIM

#define TITLE "SDL-Projection"

#ifdef CTEST
int test_main();
#endif


int main(int argc, char **argv) {
    // Initialization
    if(SDL_Init(SDL_INIT_VIDEO ) < 0) {
        printf("%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

#ifdef CTEST
    printf("Unit Test Mode\n");
    test_main();
    return 0;
#endif

    SDL_Window *win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       WIDTH,
                                       HEIGHT,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(win == NULL) {
        printf("%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
   
    // Create new renderer with FOV of 90 degrees (1,5708 rad)
    renderCtx_t* p = renderCtxNew(win, 1, M_PI / 2);
    if(p == NULL) {
        printf("%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    Error_t res = cubeNew(10, p->obj_v + 0);
    if(res > 0) return EXIT_FAILURE;
   

    eventhandler_t* eventHandler = newEventhandler(win, p);

    // center the cube
    vec_3_t v;
    v.x = -5;
    v.y = -5;
    v.z = 25;
    objectMove(p->obj_v, v);
    
    // first render
    projectObjects(p);

    while(0 == eventHandler->quitApp) {
        pollEvents(eventHandler);
    }

    // cleanup
    renderCtxFree(p);

    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}





/* ------==================------ */
/* ------====== TEST ======------ */
/* ------==================------ */

#ifdef CTEST

static int initSuite(void)
{
    return CUE_SUCCESS;
}

static int cleanupSuite(void)
{
    return CUE_SUCCESS;
}

int test_main()
{
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    CU_pSuite pSuite = CU_add_suite("SDL-Projection", initSuite, cleanupSuite);
    
    /* check if the suite was created successfully */
    if (NULL == pSuite) {
        CU_cleanup_registry();
    }

    /* add here all tests */
    if ( (NULL == CU_add_test(pSuite, "Cube: \t cubeNew interface \t", test_cubeNew)) )
    {
       CU_cleanup_registry();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    return CU_get_error();
}

#endif
