#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "render.h"

typedef struct{
    SDL_Window* window;
    render_t* render;

    bool quit_app;
    bool init_done;
} event_handler_t;

int32_t event_handler_init(event_handler_t* event_handler, SDL_Window* window, render_t* render);

void event_handler_free(event_handler_t* event_handler);

int32_t digest_events(event_handler_t* event_handler_t);

#endif