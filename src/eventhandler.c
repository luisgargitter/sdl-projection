#include "eventhandler.h"

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL_events.h>
#include <math.h>

#include "linag.h"
#include "render.h"

#define POLL_EVENT_TIMEOUT_MS 16 // 16 ms for 60 fps

event_handler_t event_handler_init(SDL_Window *w, render_t *r) {
    event_handler_t e;
    e.window = w;
    e.render = r;
    e.time_of_last_frame = 0;

    e.quit_app = false;
    e.x_angle = 0;
    e.y_angle = 0;
    return e;
}

int64_t millis() { // copy paste from stackoverflow (seems to be working)
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t)now.tv_sec) * 1000 + ((int64_t)now.tv_nsec) / 1000000;
}

int print_debug(event_handler_t *e) {
    static int i = 0;
    if (i % POLL_EVENT_TIMEOUT_MS == 0) {
        printf("\033c"); // clear screen
        printf("Offset: x: %6f, y: %6f, z: %6f\n", e->render->offset.x,
               e->render->offset.y, e->render->offset.z);
        printf("Orientation: x-axis: %6f°, y-axis: %6f°\n",
               (e->x_angle / M_PI) * 180, (e->y_angle / M_PI) * 180);
        long deltaT = millis() - e->time_of_last_frame;
        printf("Delta-time since last frame (ms): %ld | ~%.0f FPS\n", deltaT,
               1000.0 / deltaT);
    }
    i = (i + 2) % 10;

    return 0;
}

int32_t digest_events(event_handler_t *e) {
    int32_t retCode = 0;
    SDL_Event s;
    static int64_t time_since_last_frame = POLL_EVENT_TIMEOUT_MS;

    vec3_t v = vec3(0, 0, 0);
    static bool kw = false;
    static bool ka = false;
    static bool ks = false;
    static bool kd = false;
    SDL_Keycode kc;
    float mov_step = 0.0125;

    time_since_last_frame = millis() - e->time_of_last_frame;

    // smooths out time between frames
    if (time_since_last_frame > POLL_EVENT_TIMEOUT_MS)
        time_since_last_frame = POLL_EVENT_TIMEOUT_MS;
    SDL_WaitEventTimeout(&s, POLL_EVENT_TIMEOUT_MS - time_since_last_frame);

    print_debug(e);

    e->time_of_last_frame = millis();

    // adds up all movement that happened, and renders final result
    do {
        switch (s.type) {
        case SDL_QUIT:
            e->quit_app = 1;
            break;

        case SDL_WINDOWEVENT:
            /* if a resizing window event was triggered, just re-draw the object
             * to fill the empty space */
            // update center of projection
            SDL_GetWindowSize(e->window, &e->render->width, &e->render->height);
            e->render->scaled_fov = fminf(e->render->width, e->render->height) *
                                    e->render->fov_ratio;
            break;

        case SDL_MOUSEMOTION:
            // if(s.motion.state == SDL_BUTTON_LMASK) {
            e->y_angle -= ((float)s.motion.xrel) / 500.0;
            e->x_angle -= ((float)s.motion.yrel) / 500.0;

            // limiting vertical movement
            e->x_angle = (e->x_angle < -M_PI / 2) ? -M_PI / 2 : e->x_angle;
            e->x_angle = (e->x_angle > M_PI / 2) ? M_PI / 2 : e->x_angle;
            e->y_angle = fmod(e->y_angle, 2 * M_PI);
            //}
            break;

            // movement
        case SDL_KEYDOWN:
            kc = s.key.keysym.sym;
            if (kc == SDLK_w)
                kw = true;
            else if (kc == SDLK_a)
                ka = true;
            else if (kc == SDLK_s)
                ks = true;
            else if (kc == SDLK_d)
                kd = true;
            // quit
            else if (kc == SDLK_q)
                e->quit_app = 1;
            else if (kc == SDLK_g) {
                if (SDL_GetRelativeMouseMode()) // If mouse grabbed
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                else
                    SDL_SetRelativeMouseMode(SDL_TRUE);
            }
            break;

        case SDL_KEYUP:
            kc = s.key.keysym.sym;
            if (kc == SDLK_w)
                kw = false;
            else if (kc == SDLK_a)
                ka = false;
            else if (kc == SDLK_s)
                ks = false;
            else if (kc == SDLK_d)
                kd = false;
            // quit
            else if (kc == SDLK_q)
                e->quit_app = 1;
            break;

        default:
            break;
        }
    } while (SDL_PollEvent(&s) > 0);

    v.z -= kw == true ? mov_step : 0;
    v.z += ks == true ? mov_step : 0;

    v.x += ka == true ? mov_step : 0;
    v.x -= kd == true ? mov_step : 0;

    e->render->orientation = mat3_matmul(mat3_rotation(vec3(e->x_angle, 0, 0)),
                                  mat3_rotation(vec3(0, e->y_angle, 0)));

    // past movement needs to be rotated opposed to rest (order of
    // multiplication must also be reversed)
    mat3_t ro = mat3_matmul(mat3_rotation(vec3(0, -e->y_angle, 0)),
                     mat3_rotation(vec3(-e->x_angle, 0, 0)));

    e->render->offset = vec3_add(e->render->offset, mat3_apply(ro, v));

    retCode = render_frame(e->render);

    return retCode;
}
