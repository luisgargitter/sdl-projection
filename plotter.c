#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

#define TITLE "Primitives"

#define ABORT(msg); printf("Fatal SDL Error: %s\n", msg); return EXIT_FAILURE;

typedef struct {
    SDL_Renderer *r;
    int width;
    int height;
    int offsetx;
    int offsety;
    float scalex;
    float scaley;
    int res;
    int vertc;
    SDL_Point *vertv;
} PLOTTER_CTX;

PLOTTER_CTX * plotter_ctx_new(SDL_Window *w) {
    PLOTTER_CTX *p = malloc(sizeof(PLOTTER_CTX));
    if(p == NULL) abort();
    p->r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if(p->r == NULL) {
        free(p);
        return NULL;
    }
    SDL_GetWindowSize(w, &p->width, &p->height);
    p->offsetx = p->width / 2;
    p->offsety = p->height / 2;

    p->scalex = 1.0;
    p->scaley = 1.0;
    p->res = 1;

    p->vertc = (int) p->offsetx * 2 / p->res;
    p->vertv = malloc(sizeof(SDL_Point) * p->vertc);
    if(p->vertv == NULL) {
        free(p);
        return NULL;
    }
    
    return p;
}

void plotter_ctx_free(PLOTTER_CTX *p) {
    SDL_DestroyRenderer(p->r);
    free(p->vertv);
    p->vertc = 0;
    free(p);
}

float calc_polynomial(float x, int polc, float *polv) {
    float res = 0;
    for(int i = 0; i < polc; i++) {
        res += pow(x, i) * -polv[i];
    }
    return res;
}

int update_verts(PLOTTER_CTX *p, int polc, float *polv) {
    for(int i = 0; i < p->vertc; i++) {
        p->vertv[i].x = i * p->res;
        p->vertv[i].y = (int) (calc_polynomial((i * p->res - p->offsetx) * p->scalex, polc, polv) * p->scaley + p->offsety);
    }
}

int plot(PLOTTER_CTX *p, int polc, float *polv) {
    SDL_SetRenderDrawColor(p->r, 0, 0, 0, 0);
    SDL_RenderClear(p->r);
    update_verts(p, polc, polv);

    SDL_SetRenderDrawColor(p->r, 255, 255, 255, 255);

    SDL_RenderDrawLines(p->r, p->vertv, p->vertc);
    SDL_RenderDrawLine(p->r, 0, p->offsety, p->width, p->offsety);
    SDL_RenderDrawLine(p->r, p->offsetx, 0, p->offsetx, p->height);
    
    SDL_RenderPresent(p->r);

    return 0;
}

int main(int argc, char **argv) {
    // Initialization
    if(SDL_Init(SDL_INIT_VIDEO ) < 0) {
        ABORT(SDL_GetError);
    }

    SDL_Window *win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(win == NULL) {
        ABORT(SDL_GetError());
    }
    
    PLOTTER_CTX *p = plotter_ctx_new(win);

    int polc = 4;
    float polv[4] = {10.0, 1.0, -0.01, 0.0001};

    plot(p, polc, polv);

    SDL_Event e;
    while(1) {
        SDL_PollEvent(&e);
        if(e.type == SDL_QUIT) break;
        if(e.type == SDL_MOUSEMOTION) if(e.motion.state != 0) {
            p->offsetx += e.motion.xrel;
            p->offsety += e.motion.yrel;
        }

        plot(p, polc, polv);
    }

    // cleanup
    // SDL_DestroyRenderer(r);
    plotter_ctx_free(p);

    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
