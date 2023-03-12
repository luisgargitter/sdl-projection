#include "eventhandler.h"
#include <stdlib.h>


#define POLL_EVENT_TIMEOUT_MS   10



eventhandler_t* newEventhandler(renderCtx_t* pRender)
{
    eventhandler_t* obj = malloc(sizeof(eventhandler_t));

    obj->pRenderer = pRender;

    return obj;
}

Error_t pollEvents(eventhandler_t* pObj)
{
    Error_t retCode = ERR_OK;
    SDL_Event e;


    SDL_WaitEventTimeout(&e, POLL_EVENT_TIMEOUT_MS);

    return retCode;
}

