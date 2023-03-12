#include "eventhandler.h"
#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include "cube.h"

#define POLL_EVENT_TIMEOUT_MS   10


eventhandler_t* newEventhandler(SDL_Window* pWin, renderCtx_t* pRender)
{
    eventhandler_t* obj = malloc(sizeof(eventhandler_t));
    obj->pWin = pWin;
    obj->pRenderer = pRender;
    obj->quitApp = 0;
    return obj;
}

Error_t pollEvents(eventhandler_t* self)
{
    Error_t retCode = ERR_OK;
    SDL_Event e;

    vec_3_t v;
    SDL_WaitEventTimeout(&e, POLL_EVENT_TIMEOUT_MS);

    switch (e.type)
    {
        case SDL_QUIT: {
            self->quitApp = 1;
            break;
        }

        case SDL_WINDOWEVENT: {
            /* if a resizing window event was triggered, just re-draw the object to fill the empty space */
	    // update center of projection
            SDL_GetWindowSize(self->pWin, &self->pRenderer->width, &self->pRenderer->height);
            projectObjects(self->pRenderer);
            break;
        }

        case SDL_MOUSEMOTION: {
            if(e.motion.state == SDL_BUTTON_LMASK) {
                v.x = (float) e.motion.xrel / 10;
                v.y = (float) e.motion.yrel / 10;
                v.z = 0;
                objectMove(self->pRenderer->obj_v, v);
		projectObjects(self->pRenderer);
            }
            break;
        }

        case SDL_MOUSEWHEEL: {
			v.x = 0;
            v.y = 0;
            v.z = e.wheel.preciseY;
	    objectMove(self->pRenderer->obj_v, v);
	    projectObjects(self->pRenderer);
            break;
	}


        default:
        {
            break;
        }
    }

    return retCode;
}

