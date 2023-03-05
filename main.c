#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

#include "types.h"
#include "render.h"
#include "cube.h"

#define WIDTH 800
#define HEIGHT 600

#define CUBEVERTC 8 * DIM

#define TITLE "SDL-Projection"

int main(int argc, char **argv) {
    // Initialization
    if(SDL_Init(SDL_INIT_VIDEO ) < 0) {
        printf("%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(win == NULL) {
        printf("%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    renderCtx_t *p = renderCtxNew(win);
    if(p == NULL) {
        printf("%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    p->obj_c = 1;
    p->obj_v = cubeNew(10);
    vec_3_t v;
    v.x = -5;
    v.y = -5;
    v.z = 5;

    objectMove(p->obj_v, v);
    projectObjects(p);

    SDL_Event e;
    while(1) {
        SDL_PollEvent(&e);
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
    }

    // cleanup
    renderCtxFree(p);

    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
