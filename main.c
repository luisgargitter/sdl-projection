#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

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
    
    // center the cube
    vec_3_t v;
    v.x = -5;
    v.y = -5;
    v.z = 25;
    objectMove(p->obj_v, v);
    
    // first render
    projectObjects(p);

    SDL_Event e;
    while(1) {
        SDL_WaitEventTimeout(&e, 10); //wait 10ms at most for the next event
        if(e.type == SDL_QUIT) break;
        if(e.type == SDL_MOUSEMOTION) {
            if(e.motion.state == SDL_BUTTON_LMASK) {
                v.x = (float) e.motion.xrel / 10;
                v.y = (float) e.motion.yrel / 10;
                v.z = 0;
                objectMove(p->obj_v, v);
				projectObjects(p);
            }
        }
        if(e.type == SDL_MOUSEWHEEL) {
			v.x = 0;
            v.y = 0;
            v.z = e.wheel.preciseY;
			objectMove(p->obj_v, v);
			projectObjects(p);
		}
        if(e.type == SDL_WINDOWEVENT) {
            /* if a resizing window event was triggered, just re-draw the object to fill the empty space */
			// update center of projection
            SDL_GetWindowSize(win, &p->width, &p->height);
            projectObjects(p);
        }
		
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
