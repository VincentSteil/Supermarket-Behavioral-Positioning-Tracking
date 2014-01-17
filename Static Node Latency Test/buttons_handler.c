#include "buttons_handler.h"
#include "buttons_handler_priv.h"

#include "app_timer.h"
#include "nrf51_bitfields.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"

//#define DBG_MODULE_ENABLED
#include "debug_helper.h"

//assumed that buttons are pressed separately
static uint32_t downStartTime = 0;

BTNS_Data *btns_init(BTNS_Data *context, BTNS_PrivData *pData)
{
    context->priv = pData;
    int i = 0;
    for (; i < pData->configSize; ++i) {
        pData->config[i].btnChannelNo = BTNS_UnassignedChannel;//set sentinel
        pData->config[i].btnGpio = BTNS_UnassignedGpio;
    }

    NVIC_EnableIRQ(GPIOTE_IRQn);

    return context;
}

int btns_addGpioteBtn(BTNS_Data *context, uint32_t pinNo, uint32_t channelNo, BTNS_ButtonEventHandler hndlr, void *hndlrData)
{
    //find an empty place
    BTNS_PrivData *p = context->priv;
    int i = 0;
    for (; i < p->configSize; ++i) {
        BTNS_Config *cfg = &(p->config[i]);
        if (BTNS_UnassignedChannel == cfg->btnChannelNo) {
            cfg->btnChannelNo = channelNo;
            cfg->btnGpio = pinNo;
            cfg->handler = hndlr;
            cfg->hndlrData = hndlrData;
            break;
        }
    }

    //has a free space been found?
    if (i == p->configSize)
        return -1;

    //button ports configured as input configured
    nrf_gpio_cfg_input(pinNo, NRF_GPIO_PIN_PULLUP);
    //gpiote events are triggered on both edges
    nrf_gpiote_event_config(channelNo, pinNo, NRF_GPIOTE_POLARITY_TOGGLE);
    //set appropriate inteset bits
    NRF_GPIOTE->INTENSET  |= GPIOTE_INTENSET_IN0_Set << (GPIOTE_INTENSET_IN0_Pos + channelNo);

    dbg_debugModule("Added ch: %d, gpio: %d\n", channelNo, pinNo);

    return 0;
}


int btns_rmGpioteBtn(BTNS_Data *context, uint32_t pinNo, uint32_t channelNo)
{
    //find record
    BTNS_PrivData *p = context->priv;
    int i = 0;
    for (; i < p->configSize; ++i) {
        if (channelNo == p->config[i].btnChannelNo &&
                pinNo == p->config[i].btnGpio) {
            p->config[i].btnChannelNo = BTNS_UnassignedChannel;
            p->config[i].btnGpio = BTNS_UnassignedGpio;
            break;
        }
    }

    if (i == p->configSize)//was not found
        return -1;

    //clear intents
    NRF_GPIOTE->INTENCLR |= GPIOTE_INTENCLR_IN0_Clear << (GPIOTE_INTENCLR_IN0_Pos + channelNo);

    return 0;
}


void btns_gpioTeHandler(BTNS_Data *context)
{
    const BTNS_PrivData *p = context->priv;
    int i = 0;
    for (; i <p->configSize; ++i) {
        const BTNS_Config *cfg = &(p->config[i]);
        if (BTNS_UnassignedChannel != cfg->btnChannelNo) {
            if (1 == NRF_GPIOTE->EVENTS_IN[cfg->btnChannelNo]){                            // Check if GPIO2 being pulled high triggered the interrupt
                NRF_GPIOTE->EVENTS_IN[cfg->btnChannelNo] = 0;                             // Reset interrupt

                uint32_t isReleased = nrf_gpio_pin_read(cfg->btnGpio);
                //for a button, value 0 means it's pressed, 1 - released
                if (isReleased) {
                    uint32_t upTime;
                    app_timer_cnt_get(&upTime);
                    if (upTime - downStartTime > BTNS_LONG_COUNT)
                        cfg->handler(cfg->hndlrData, BTNS_ButtonLongPress);
                    else
                        cfg->handler(cfg->hndlrData, BTNS_ButtonShortPress);
                }
                else {
                    app_timer_cnt_get(&downStartTime);
                    dbg_debugModule("Btn down\n");
                }
            }
        }
    }
}
