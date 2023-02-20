#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

#define DIM 3
#define CUBEVERTC 8 * DIM

#define TITLE "Primitives"

#define ABORT(msg); printf("Fatal SDL Error: %s\n", msg); return EXIT_FAILURE;

typedef struct {
    SDL_Renderer *r;
    int width;
    int height;
    size_t vertc;
    SDL_Vertex *vertv;
} RENDER_CTX;

RENDER_CTX * render_ctx_new(SDL_Window *w) {
    RENDER_CTX *p = malloc(sizeof(RENDER_CTX));
    if(p == NULL) abort();
    p->r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if(p->r == NULL) {
        free(p);
        return NULL;
    }
    SDL_GetWindowSize(w, &p->width, &p->height);

    p->vertc = 0;
    p->vertv = NULL; 
    
    return p;
}

void render_ctx_free(RENDER_CTX *p) {
    SDL_DestroyRenderer(p->r);
    free(p->vertv);
    p->vertc = 0;
    free(p);
}

int project_points(int pointc, float *pointv, SDL_Vertex *vertv) {
    for(int i = 0; i < pointc; i++) {
        vertv[i].position.x = pointv[i + 0] / pointv[i + 2];
        vertv[i].position.y = pointv[i + 1] / pointv[i + 2];
    }

    return 0;
}

float * cube_init(float edgelen) {
    float *c = malloc(sizeof(float) * CUBEVERTC);

    int ind = 0;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            for(int k = 0; k < 2; k++) {
                c[ind++] = i * edgelen;
                c[ind++] = j * edgelen;
                c[ind++] = k * edgelen; 
            }
        }
    }

    return c;
}

void cube_move(float *c, float x, float y, float z) {
    for(int i = 0; i < CUBEVERTC; i+=DIM) {
        c[i] += x;
        c[i + 1] += y;
        c[i + 2] += z;
    }
}

void cube_rotate(float *c, float radians[3]) {
    for(int i = 0; i < CUBEVERTC; i+=3) {
        c[i] = c[i] + c[i] * cosf(radians[1]) + c[i] * -sinf(radians[1]) + c[i] * cosf(radians[2]) + c[i] * sinf(radians[2]);
        c++;
        c[i] = c[i] + c[i] * cosf(radians[0]) + c[i] * sinf(radians[0]) + c[i] * cosf(radians[2]) + c[i] * -sinf(radians[2]);
        c++;
        c[i] = c[i] + c[i] * cosf(radians[0]) + c[i] * -sinf(radians[0]) + c[i] * cosf(radians[1]) + c[i] * sinf(radians[1]);
    }
}

int main(int argc, char **argv) {
    // Initialization
    if(SDL_Init(SDL_INIT_VIDEO ) < 0) {
        ABORT(SDL_GetError());
    }

    SDL_Window *win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(win == NULL) {
        ABORT(SDL_GetError());
    }
    
    RENDER_CTX *p = render_ctx_new(win);
    if(p == NULL) {
        ABORT(SDL_GetError());
    }

    float edgelen = 100.0;
    float *cube = cube_init(edgelen);
    cube_move(cube, edgelen/2, edgelen/2, 100);
    p->vertc = CUBEVERTC;
    p->vertv = malloc(sizeof(SDL_Vertex) * CUBEVERTC);

    project_points(CUBEVERTC, cube, p->vertv);
    SDL_SetRenderDrawColor(p->r, 255, 255, 255, 255);
    SDL_RenderGeometry(p->r, NULL, p->vertv, p->vertc, NULL, 0);
    SDL_RenderPresent(p->r);

    SDL_Event e;
    while(1) {
        SDL_PollEvent(&e);
        if(e.type == SDL_QUIT) break;
    }

    // cleanup
    // SDL_DestroyRenderer(r);
    free(cube);

    render_ctx_free(p);

    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
