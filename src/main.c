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
#include "linag.h"
#include "render.h"
#include "asset.h"
#include "object.h"
#include "eventhandler.h"
#include "test.h"
#include "projection_error.h"

#define TITLE "SDL-Projection"
#define WIDTH 800
#define HEIGHT 600

#define FOV (M_PI / 2)

#define main_def_handle(f) 		                \
	switch(f) {					\
	case 0: break;				        \
	case -1:					\
		perror(argv[0]);		        \
		assert(0);				\
		return EXIT_FAILURE;	                \
	case -2:					\
		fprintf(stderr, "%s: %s\n", argv[0], SDL_GetError()); \
		assert(0);				\
		return EXIT_FAILURE;	                \
	default:					\
		assert(0);				\
}

#ifdef CTEST
int test_main();
#endif

int sdl_window_setup(SDL_Window **win) {
    // Initialization
    if(SDL_Init(SDL_INIT_VIDEO ) < 0) return -2;
    
    *win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if(*win == NULL) return -2;

    return 0;
}

int sdl_event_handler_setup(event_handler_t* eh, SDL_Window* win, render_t* r) {
    return event_handler_init(eh, win, r);
}

int main(int argc, char **argv) {
    #ifdef CTEST
    printf("Unit Test Mode\n");
    test_main();
    return 0;
    #endif
    
    SDL_Window* w;
    render_t r;
  event_handler_t e;

	main_def_handle(sdl_window_setup(&w));

    SDL_SetRelativeMouseMode(SDL_TRUE);
	
	main_def_handle(render_init(&r, w, FOV));

	main_def_handle(event_handler_init(&e, w, &r));

    FILE *f;
    asset_t a;
    
	if (argc < 2) {
    	printf("%s %s\n", argv[0], "[file]");
		return EXIT_SUCCESS;
	} else {
    	f = fopen(argv[1], "rb");
  	}

	if(f == NULL) {
		perror(argv[0]);
		assert(0);
		return EXIT_FAILURE;
	}

    main_def_handle(asset_load_obj(f, &a));

    fclose(f);

	mat3_t m = mat3_rotation(vec3(0, 0, 0));
     
    render_add_object(&r, &a, m, vec3(0, 0, 0));

    while(e.quit_app == false) digest_events(&e);

	render_cleanup(r);

    SDL_DestroyWindow(w);
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
