#ifndef BUTTONS_HANDLER_PRIV_H
#define BUTTONS_HANDLER_PRIV_H

#include "buttons_handler.h"
#include "buttons_handler_common.h"

typedef enum {
    BTNS_UnassignedChannel = 0xFFFFFFFF,
    BTNS_UnassignedGpio = 0xFFFFFFFF
} BTNS_Constants;

typedef struct {
    uint32_t btnGpio;
    uint32_t btnChannelNo;
    BTNS_ButtonEventHandler handler;
    void *hndlrData;
} BTNS_Config;

typedef struct {
    int configSize;
    BTNS_Config *config;
} BTNS_PrivData;

#endif // BUTTONS_HANDLER_PRIV_H
