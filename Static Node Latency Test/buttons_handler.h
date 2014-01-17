#ifndef BUTTONS_HANDLER_H
#define BUTTONS_HANDLER_H

#include <stdint.h>

#include "buttons_handler_priv.h"
#include "buttons_handler_common.h"

#define BTNS_LONG_COUNT 20   //coupled with APP_TIMER_PRESCALER. Number of counts since press, when press is considered as a long press.

/**
  * Class used for detection of short/long button presses. It doesn't do debouncing (which seems unnecessary
  * in case of our board after few tests).
  * Only one button can be pressed at a time.
  * Class uses app_timer for time calculations.
  */
typedef struct {
    BTNS_PrivData *priv;
} BTNS_Data;

#define BTNS_INIT(BTNS_NO, BTNS_OUT_PTR)                \
    BTNS_Config btnsConfig[BTNS_NO];                    \
    BTNS_PrivData btnsPrivData = {BTNS_NO, btnsConfig}; \
    BTNS_Data btnsData;                                 \
    BTNS_OUT_PTR = btns_init(&btnsData, &btnsPrivData);

BTNS_Data *btns_init(BTNS_Data *context, BTNS_PrivData *pData);

/**
 * @brief btns_addGpioteBtn - adds new pinNo, channelNo to be tracked as button input
 *  Function configures pin as a PULL UP input, and sets appropriate intenset
 *  @note NVIC_EnableIRQ(GPIOTE_IRQn) still has to be called somewhere in a program.
 * @param pinNo
 * @param channelNo
 */
int btns_addGpioteBtn(BTNS_Data *context, uint32_t pinNo, uint32_t channelNo, BTNS_ButtonEventHandler hndlr, void *hndlrData);

/**
 * @brief btns_rmGpioteBtn
 * @param pinNo
 * @return
 */
int btns_rmGpioteBtn(BTNS_Data *context, uint32_t pinNo, uint32_t channelNo);

/**
 * @brief btns_gpioTeHandler
 */
void btns_gpioTeHandler(BTNS_Data *context);

#endif // BUTTONS_HANDLER_H
