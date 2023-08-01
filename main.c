// libraries in prelude
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

// explicitly linked libraries
#include <math.h>
#include <SDL2/SDL.h>

// files in project
#include "render.h"
#include "asset.h"
#include "object.h"
#include "eventhandler.h"
#include "test.h"
#include "projection_error.h"

#define PROJECTION_DEBUG // for more detailed crashreports

#define TITLE "SDL-Projection"
#define WIDTH 800
#define HEIGHT 600

#ifdef CTEST
int test_main();
#endif


int main(int argc, char **argv) {
    #ifdef CTEST
    printf("Unit Test Mode\n");
    test_main();
    return 0;
    #endif
    
    // Initialization
    if(SDL_Init(SDL_INIT_VIDEO ) < 0) info_and_abort(SDL_GetError());

    SDL_Window *win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(win == NULL) info_and_abort(SDL_GetError());

    render_t* r = malloc(sizeof(*r));
    if(r == NULL) info_and_abort(NULL);
    
    // Create new renderer with FOV of 90 degrees (1,5708 rad)
    if(render_init(r, win, M_PI / 2) < 0) info_and_abort(NULL);

    // TODO: fully reintegrate eventhandler to enable movement
    event_handler_t* eh = malloc(sizeof(*eh));
    if(eh == NULL) info_and_abort(NULL);
    event_handler_init(eh, win, r);

    FILE* f = fopen("test_assets/teapot.obj", "rb");
    if(f == NULL) info_and_abort(strerror(errno));

    asset_t* a = malloc(sizeof(*a));
    if(a == NULL) info_and_abort(NULL);

    asset_load_obj(f, a);
    fclose(f);

    vec_3_t c1 = {.x = 0.71, .y = 0, .z = -0.71};
    vec_3_t c2 = {.x = 0, .y = 1, .z = 0};
    vec_3_t c3 = {.x = 0.71, .y = 0, .z = 0.71};
    matrix_3x3_t m = {c1, c2, c3};

    vec_3_t v = {0, -3, 10};
    
    render_add_object(r, a, m, v);

    
    while(eh->quit_app == false) {
        digest_events(eh);
    }
    
    //if(projectObjects(r)) print_n_abort(SDL_GetError());

    // cleanup
    event_handler_free(eh);
    free(eh);

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
