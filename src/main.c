// libraries in prelude
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// explicitly linked libraries
#include <SDL2/SDL.h>
#include <math.h>

// files in project
#include "asset.h"
#include "eventhandler.h"
#include "linag.h"
#include "object.h"
#include "projection_error.h"
#include "render.h"

#define TITLE "SDL-Projection"
#define WIDTH 800
#define HEIGHT 600

#define FOV (M_PI / 2)

int main(int argc, char **argv) {
    FILE *f;

    if (argc < 2) {
        printf("%s %s\n", argv[0], "[file]");
        return EXIT_SUCCESS;
    } else {
        f = fopen(argv[1], "rb");
    }

    asset_t a;

    if (f == NULL) {
        perror(argv[0]);
        assert(0);
        return EXIT_FAILURE;
    }


    asset_load_obj(f, &a);

    fclose(f);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "%s: %s\n", argv[0], SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(
        TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        fprintf(stderr, "%s: %s\n", argv[0], SDL_GetError());
        return EXIT_FAILURE;
    }

    if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1) {
        fprintf(stderr, "%s: %s\n", argv[0], SDL_GetError());
        return EXIT_FAILURE;
    }



    render_t render = render_init(window, FOV);

    event_handler_t event_handler = event_handler_init(window, &render);

    mat3_t m = mat3_rotation(vec3(0, 0, 0));

    render_add_object(&render, &a, m, vec3(0, 0, 0));

    while (event_handler.quit_app == false)
        digest_events(&event_handler);

    render_cleanup(render);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}