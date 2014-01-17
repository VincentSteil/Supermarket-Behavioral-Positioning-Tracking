#ifndef BUTTONS_HANDLER_COMMON_H
#define BUTTONS_HANDLER_COMMON_H

typedef enum {
    BTNS_ButtonShortPress,
    BTNS_ButtonLongPress
} BTNS_EventType;

typedef void (*BTNS_ButtonEventHandler)(void *handlerData, BTNS_EventType evType);


#endif // BUTTONS_HANDLER_COMMON_H
