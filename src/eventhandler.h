#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "render.h"

typedef struct {
	SDL_Window *window;
	render_t *render;

	int64_t time_of_last_frame;

	bool quit_app;
	bool init_done;

	float x_angle;
	float y_angle;
} event_handler_t;

int32_t event_handler_init(event_handler_t * event_handler, SDL_Window * window,
			   render_t * render);

int32_t digest_events(event_handler_t * event_handler_t);

#endif