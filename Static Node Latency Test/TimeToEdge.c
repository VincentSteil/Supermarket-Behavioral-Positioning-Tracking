// TimeToEdge.c
// Vincent Steil
// 13.11.2013

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "math.h"
#include "nrf_gpiote.h"
#include "TimeToEdge.h"
#define DBG_MODULE_ENABLED
#include "debug_helper.h"
#include "uart.h"

void timer1_init(void)
{

    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;                    // Start 16 MHz crystal oscillator
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)             // Wait for the external oscillator to start up
    {
        // Do nothing.
    }

    
    NRF_TIMER1->MODE        = TIMER_MODE_MODE_Timer;        // Set the timer in Timer Mode
    NRF_TIMER1->PRESCALER   = 0;                            // Set timer freq to 16 MHz / 2^0 -> 0.0625us

    //!NOTE: Below line doesn't work - timer stays in 16-bit mode
    //NRF_TIMER1->BITMODE     = TIMER_BITMODE_BITMODE_24Bit;  // 24-bit mode

    //hack for extending timer range - whenever timer overflows, interrupt handler is invoked and ext_counter incremented
    NRF_TIMER1->BITMODE     = TIMER_BITMODE_BITMODE_16Bit;  // we leave 16-bit mode and add additional counter
    NRF_TIMER1->TASKS_STOP      = 1;
    NRF_TIMER1->TASKS_CLEAR     = 1;

    //Enable Timer interrupt
    NRF_TIMER1->CC[1]           = 0;
    NRF_TIMER1->INTENCLR = 0xffffffffUL;
    NRF_TIMER1->INTENSET = (TIMER_INTENSET_COMPARE1_Set << TIMER_INTENSET_COMPARE1_Pos);
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    NVIC_EnableIRQ(TIMER1_IRQn);
}

void gpio_init(void)
{
    nrf_gpio_cfg_input(11, NRF_GPIO_PIN_PULLDOWN);

    NVIC_EnableIRQ(GPIOTE_IRQn);                                // Enable interrupt:

    nrf_gpiote_event_config(0, 11, NRF_GPIOTE_POLARITY_LOTOHI); // Channel[0], Pin11 (GPIO2), interrupt on toggle
}

static void enable_edge_trigger(void){
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;                 // Enable the edge triggered interrupt on GPIO2
}

static void disable_edge_trigger(void){
    NRF_GPIOTE->INTENCLR  = GPIOTE_INTENCLR_IN0_Clear << GPIOTE_INTENCLR_IN0_Pos;               // Disable the edge triggered interrupt on GPIO2
}

volatile char timer_delay_ready = 0;
static volatile uint32_t ext_counter = 0;
volatile uint32_t timer_delay = 0;

static void timer1_start(void){
    ext_counter = 0;
    NRF_TIMER1->TASKS_CLEAR = 1;                            // Clear timer1
    NRF_TIMER1->TASKS_START = 1;                            // Start timer1
}

void TIMER1_IRQHandler(void){

    if(NRF_TIMER1->EVENTS_COMPARE[1] == 1){
        NRF_TIMER1->EVENTS_COMPARE[1] = 0;
        ext_counter++;
    }
}

void tte_gpioteIrqHandler() {
    if(NRF_GPIOTE->EVENTS_IN[0] == 1) {                            // Check if GPIO2 being pulled high triggered the interrupt
        NRF_GPIOTE->EVENTS_IN[0] = 0;                             // Reset interrupt
        NRF_TIMER1->TASKS_CAPTURE[0]  = 1; 
        nrf_gpio_pin_toggle(0);

                                   // Write time delay to CC[0] register
        timer_delay                   = NRF_TIMER1->CC[0];            // Copy CC[0] register to timer_delay
        timer_delay += ((ext_counter) << 16);
        NRF_TIMER1->CC[0] = 0;
        NRF_TIMER1->TASKS_CLEAR = 1;
        NRF_TIMER1->TASKS_STOP        = 1;                            // Stop Timer1
        disable_edge_trigger();
        // TRIGGER FUNCTION TO SEND OUT DATA timer_delay
        timer_delay_ready += 1;
        dbg_debugModule("Interrupt over\n");
        dbg_debugModule("Interrupt latency: %d\n", timer_delay);
    }
}

void start_waiting_for_edge(void){
    timer1_start();
    enable_edge_trigger();
}
