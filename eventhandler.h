#pragma once    // include guard

#include "types.h"
#include "render.h"


typedef struct{
    SDL_Window* pWin;
    render_t* pRenderer;

    u8 quitApp;
    u8 initDone;
    
} eventhandler_t;

eventhandler_t* newEventhandler(SDL_Window* pWin, render_t* pRender);
void freeEventhandler(eventhandler_t* self);
Error_t pollEvents(eventhandler_t* self);
