#pragma once    // include guard

#include "types.h"
#include "render.h"

typedef void (*TriggerEvent_t)(void*);

typedef struct
{
    u32 eventType;
    TriggerEvent_t pFnc;
} event_t;



typedef struct
{
    u16 registeredEvents;
    event_t eventList[16];

    renderCtx_t* pRenderer;

    u8 quitApp;
    u8 initDone;
    
} eventhandler_t;

eventhandler_t* newEventhandler(renderCtx_t* pRender);
Error_t pollEvents(eventhandler_t* pObj);
