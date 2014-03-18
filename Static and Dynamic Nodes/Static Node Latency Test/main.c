#include <stdint.h>

#include "app_common/app_timer.h"
#include "app_common/app_uart.h"
#include "nrf_gpio.h"

#include "buttons_handler.h"

//enable module debug
#define DBG_MODULE_ENABLED
#include "debug_helper.h"
//functions for signal emission
#include "TimeToEdge.h"

#include "uart.h"


//app timer settings
//timer prescaler is 12-bit -> max value is 4095
//when set to 4095, gotten freq is 8Hz
//we need 1 timer for LED and 1 for radio protool
#define APP_TIMER_PRESCALER             4095
#define APP_TIMER_MAX_TIMERS            3
#define APP_TIMER_OP_QUEUE_SIZE         6

//soft uart settings - for communication with upper branch master
//gpio P0.6 == GPIO-0
//gpio P0.7 == GPIO-1
#define SOFT_UART_RX_TX_PIN             6
#define SOFT_UART_TX_ENABLE_PIN         7
#define SOFT_UART_TX_BUFF_SIZE			128
#define SOFT_UART_RX_BUFF_SIZE			128

//hardware uart settings - used only for debugging
#define HW_UART_TX_GPIO_NO              23
#define HW_UART_RX_GPIO_NO              22
#define UART_TX_BUFF_SIZE               128
#define UART_RX_BUFF_SIZE             	128


///////////////////
///application
///////////////////

//application erro handler
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    (void)p_file_name;
    dbg_debugGlobal("M: Error %ud, lNo %ud\n", (unsigned int)error_code, (unsigned int)line_num);
}

///////////////////
///interrupt handlers
///////////////////

//data holder for buttons management
BTNS_Data *gbtnsDataPtr;

//handler for gpio events - 0 for TOF measurements, 1, 2 for buttons
void GPIOTE_IRQHandler(void)
{
    //handle time of flight interrupts
    tte_gpioteIrqHandler();

    //handle button intrrupts
    btns_gpioTeHandler(gbtnsDataPtr);
}

///////////////////
/// HFCLK needed for Radio
///////////////////

void hfclkInit() {
    // Start 16 MHz crystal oscillator.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    // Wait for the external oscillator to start up.
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) {
        // Do nothing.
    }
}

#include "uart.h"

#define TRIGGER_GPIO_PIN_NO 12
#define TX_PIN_NO 23
#define RX_PIN_NO 22

#define TD_BTN_S1_IO_NO        1
#define TD_BTN_S1_IO_CHANNEL   1
//#define TD_BTN_S2_IO_NO        14
//#define TD_BTN_S2_IO_CHANNEL   2

#define LED_GPIO_NO                      0

char cntr = 0;

static void termdev_priv_btnPress(void *bD, BTNS_EventType evType) {
    dbg_debugModule("BTNPress\n");
    start_waiting_for_edge();
    nrf_gpio_pin_set(12);
    nrf_gpio_pin_toggle(TRIGGER_GPIO_PIN_NO);
    nrf_gpio_pin_toggle(LED_GPIO_NO);
}

int main(void)
{
    //configure trigger GPIO
    nrf_gpio_cfg_output(TRIGGER_GPIO_PIN_NO);
    nrf_gpio_pin_clear(TRIGGER_GPIO_PIN_NO);
    nrf_gpio_cfg_output(12);
    nrf_gpio_pin_clear(12);
    //prepare led GPIO by setting its GPIO to output
    nrf_gpio_cfg_output(LED_GPIO_NO);

    //configure UART (38400, 8N1, appropriate ports)
    uart_config(TX_PIN_NO, RX_PIN_NO);
    dbg_setDebugFct(uart_putstring_nonl);

    dbg_debugModule("Starting...\n");

    //initialize TOF measurement timer
    timer1_init();

    //initialize TOF GPIO
    gpio_init();

    //initialize HFCLK for radio (TimeToEdge uses it as well, but configures elsewhere)
    //hfclkInit();

    BTNS_INIT(1, gbtnsDataPtr);
    //init buttons handling
    btns_addGpioteBtn(gbtnsDataPtr, TD_BTN_S1_IO_NO, TD_BTN_S1_IO_CHANNEL, termdev_priv_btnPress, gbtnsDataPtr);
//    btns_addGpioteBtn(gbtnsDataPtr, TD_BTN_S2_IO_NO, TD_BTN_S2_IO_CHANNEL, termdev_priv_btnPress, gbtnsDataPtr);

    __enable_irq();

    dbg_debugModule("Started.\n");

    uint32_t counter = 0;
    while (1 == 1) {
        if (0 != timer_delay_ready) {
            timer_delay_ready = 0;
            dbg_debugModule("STOPPED: TOF: %d\n", timer_delay);
        }

    }

    return 0;
}

