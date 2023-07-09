#include "eventhandler.h"
#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include "cube.h"

#define POLL_EVENT_TIMEOUT_MS   16 // 16 ms for 60 fps


eventhandler_t* newEventhandler(SDL_Window* pWin, render_t* pRender)
{
    eventhandler_t* obj = malloc(sizeof(eventhandler_t));
    obj->pWin = pWin;
    obj->pRenderer = pRender;
    obj->quitApp = 0;
    return obj;
}

void freeEventhandler(eventhandler_t* self)
{
    free(self);
}

Error_t pollEvents(eventhandler_t* self)
{
    Error_t retCode = ERR_OK;
    SDL_Event e;

    vec_3_t v = {.x = 0, .y = 0, .z = 0};
    SDL_WaitEventTimeout(&e, POLL_EVENT_TIMEOUT_MS); // TODO: make timeout dynamic, by subtracting time needed by calculations
    // adds up all movement that happened, and renders final result
    do {
        switch (e.type)
        {
            case SDL_QUIT:
                self->quitApp = 1;
                break;
            

            case SDL_WINDOWEVENT:
                /* if a resizing window event was triggered, just re-draw the object to fill the empty space */
	        // update center of projection
                SDL_GetWindowSize(self->pWin, &self->pRenderer->width, &self->pRenderer->height);
            break;
        

            case SDL_MOUSEMOTION:
                if(e.motion.state == SDL_BUTTON_LMASK) {
                    v.x += (float) e.motion.xrel / 10;
                    v.y += (float) e.motion.yrel / 10;
                }
                break;

            case SDL_MOUSEWHEEL:              
                v.z += e.wheel.preciseY;
                break;


            default:
                break;
        

    }
    } while(SDL_PollEvent(&e) > 0);

    //objectMove(self->pRenderer->obj_v, v);
    projectObjects(self->pRenderer);

    return retCode;
}

