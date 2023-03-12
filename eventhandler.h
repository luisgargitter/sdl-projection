#pragma once    // include guard

#include "types.h"
#include "render.h"


typedef struct
{
    SDL_Window* pWin;
    renderCtx_t* pRenderer;

    u8 quitApp;
    u8 initDone;
    
} eventhandler_t;

eventhandler_t* newEventhandler(SDL_Window* pWin, renderCtx_t* pRender);
Error_t pollEvents(eventhandler_t* self);
