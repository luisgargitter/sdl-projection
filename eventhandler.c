#include "eventhandler.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <SDL2/SDL_events.h>

#include "linag.h"
#include "render.h"

#define POLL_EVENT_TIMEOUT_MS   16 // 16 ms for 60 fps


int32_t event_handler_init(event_handler_t* e, SDL_Window* w, render_t* r) {
    e->window = w;
    e->render = r;
    e->time_of_last_frame = 0;

    e->quit_app = false;

    return 0;
}

void event_handler_free(event_handler_t* e) {
    render_free(e->render);
    free(e->render);
}

int64_t millis() { // copy paste from stackoverflow (seems to be working)
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
}

int32_t digest_events(event_handler_t* e) {
    int32_t retCode = 0;
    SDL_Event s;
    static float angx = 0;
    static float angy = 0;
    static int64_t time_since_last_frame = POLL_EVENT_TIMEOUT_MS;

    vec_3_t v = {0, 0, 0};

    if(time_since_last_frame > POLL_EVENT_TIMEOUT_MS) time_since_last_frame = POLL_EVENT_TIMEOUT_MS;
    SDL_WaitEventTimeout(&s, POLL_EVENT_TIMEOUT_MS - time_since_last_frame);
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
                e->render->scaled_fov = fminf(e->render->width, e->render->height) * e->render->fov_ratio;
                break;
        
            case SDL_MOUSEMOTION:
                if(s.motion.state == SDL_BUTTON_LMASK) {
                    angx += (float) s.motion.xrel / 200.0;
                    angy += (float) s.motion.yrel / 200.0; // minus because of the coordinate system being flipped, (0|0) being top left not bottom left
                } else if (s.motion.state == SDL_BUTTON_RMASK) {
                  vec_3_t vector = {
                    (float)s.motion.xrel / 10,
                    (float)s.motion.yrel / -10,
                    0};
                  //vector = matrix_3x3_apply(e->render->orientation, vector);
                  v = vec_3_add(v, vector);
                }
                break;

            case SDL_MOUSEWHEEL:              
                v.z += s.wheel.preciseY;
                break;

            default:
                break;
        }
    } while(SDL_PollEvent(&s) > 0);

    e->render->orientation = matrix_3x3_multiply(
        matrix_3x3_rotation(angy, 0, 0), matrix_3x3_rotation(0, angx, 0));
    e->render->offset = vec_3_add(e->render->offset, matrix_3x3_apply(matrix_3x3_multiply(matrix_3x3_rotation(0, -angx, 0), matrix_3x3_rotation(-angy, 0, 0)), v));

    int64_t t = millis();
    time_since_last_frame = t - e->time_of_last_frame;
    e->time_of_last_frame = t;

    // rotating the first object along the Y-axis (vertical axis) for orientation purposes.
    static float r = 0;
    r += 0.01;
    e->render->objects->orientation = matrix_3x3_rotation(0, r, 0);

    retCode = projectObjects(e->render);

    return retCode;
}

