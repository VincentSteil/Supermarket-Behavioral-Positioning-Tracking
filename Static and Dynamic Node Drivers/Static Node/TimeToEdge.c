// TimeToEdge.c
// Vincent Steil
// Measures the time between calling start_waiting_for_edge() and an incoming rising edge in GPIO2 


#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "math.h"
#include "nrf_gpiote.h"
#include "TimeToEdge.h"

// initialise timer1 with period 1/16MHz for max resolution
void timer1_init(void)
{
        
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;                                                    // Start 16 MHz crystal oscillator
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)                                             // Wait for the external oscillator to start up
    {
        // Do nothing.
    }
    
    NRF_TIMER1->MODE        = TIMER_MODE_MODE_Timer;                                        // Set the timer in Timer Mode  
    NRF_TIMER1->PRESCALER   = 0;                                                            // Set timer freq to 16 MHz / 2^0 -> 0.0625us    
    NRF_TIMER1->BITMODE     = TIMER_BITMODE_BITMODE_24Bit;                                  // 24-bit mode

}

// start timer1 measurement
void timer1_start(void){
    NRF_TIMER1->TASKS_CLEAR = 1;                                                            // Clear timer1
    NRF_TIMER1->TASKS_START = 1;                                                            // Start timer1
}

// initialise GPIO1
void gpio_init(void)
{
    nrf_gpio_cfg_input(11, NRF_GPIO_PIN_PULLDOWN);

    NVIC_EnableIRQ(GPIOTE_IRQn);                                                            // Enable interrupt:

    nrf_gpiote_event_config(0, 11, NRF_GPIOTE_POLARITY_TOGGLE);                             // Channel[0], Pin11 (GPIO2), interrupt on toggle 
}

// get ready to trigger on incoming edge
void enable_edge_trigger(void){  
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;             // Enable the edge triggered interrupt on GPIO2
}

// disable the edge trigger
void disable_edge_trigger(void){  
    NRF_GPIOTE->INTENCLR  = GPIOTE_INTENCLR_IN0_Clear << GPIOTE_INTENCLR_IN0_Pos;           // Disable the edge triggered interrupt on GPIO2
}

volatile uint32_t timer_delay;

// rising edge interrupt handler
void GPIOTE_IRQHandler(void)
{
    if(NRF_GPIOTE->EVENTS_IN[0] == 1){                                                      // Check if GPIO2 being pulled high triggered the interrupt
        NRF_GPIOTE->EVENTS_IN[0] = 0;                                                       // Reset interrupt
    }

    
    nrf_gpio_pin_toggle(0);
    
    NRF_TIMER1->TASKS_CAPTURE[0]  = 1;                                                      // Write time delay to CC[0] register
    timer_delay                   = NRF_TIMER1->CC[0];                                      // Copy CC[0] register to timer_delay
    NRF_TIMER1->TASKS_STOP        = 1;                                                      // Stop Timer1 
    disable_edge_trigger();
    // TRIGGER FUNCTION TO SEND OUT DATA timer_delay
    
}

// call this function if you want to start measuring the time to the next edge
void start_waiting_for_edge(void){
    timer1_start();
    enable_edge_trigger();
}
