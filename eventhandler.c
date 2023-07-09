#include "eventhandler.h"

#include <stdlib.h>
#include <SDL2/SDL_events.h>

#include "types.h"
#include "render.h"

#define POLL_EVENT_TIMEOUT_MS   16 // 16 ms for 60 fps


int32_t event_handler_init(event_handler_t* e, SDL_Window* w, render_t* r) {
    e->window = w;
    e->render = r;
    e->quit_app = 0;

    return 0;
}

void event_handler_free(event_handler_t* e) {
    render_free(e->render);
}

int32_t digest_events(event_handler_t* e) {
    Error_t retCode = ERR_OK;
    SDL_Event s;

    vec_3_t v = {0, 0, 0};
    
    SDL_WaitEventTimeout(&s, POLL_EVENT_TIMEOUT_MS); // TODO: make timeout dynamic, by subtracting time needed by calculations
    // adds up all movement that happened, and renders final result
    do {
        switch (s.type)
        {
            case SDL_QUIT:
                e->quit_app = 1;
                break;
            

            case SDL_WINDOWEVENT:
                /* if a resizing window event was triggered, just re-draw the object to fill the empty space */
	        // update center of projection
                SDL_GetWindowSize(e->window, &e->render->width, &e->render->height);
            break;
        

            case SDL_MOUSEMOTION:
                if(s.motion.state == SDL_BUTTON_LMASK) {
                    v.x += (float) s.motion.xrel / 10;
                    v.y -= (float) s.motion.yrel / 10; // minus because of the coordinate system being flipped, (0|0) being top left not bottom left
                }
                break;

            case SDL_MOUSEWHEEL:              
                v.z += s.wheel.preciseY;
                break;


            default:
                break;
        

    }
    } while(SDL_PollEvent(&s) > 0);
 
    e->render->offset = v;
    projectObjects(e->render);

    return retCode;
}

