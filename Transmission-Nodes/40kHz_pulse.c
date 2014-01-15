// 40kHz_pulse.c
// Vincent Steil

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "math.h"
#include "simple_uart.h"
#include "nrf_gpiote.h"
#include "40kHz_pulse.h"

// configure GPIO2 (Pin11) as output pin
void OutPin_Init(void){
    nrf_gpio_cfg_output(11);
}

// initialise timer1 to 12.5us interrupts
void timer1_init(void){
        
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;                        // Start 16 MHz crystal oscillator
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)                 // Wait for the external oscillator to start up
    {
        // Do nothing.
    }
    
    NRF_TIMER1->TASKS_STOP      = 1;
    NRF_TIMER1->MODE            = TIMER_MODE_MODE_Timer;        // Set the timer in Timer Mode   
    NRF_TIMER1->PRESCALER       = 3;                            // Prescaler 2 produces 2Mhz Hz timer frequency => 1 tick = 32 us.  
    NRF_TIMER1->BITMODE         = TIMER_BITMODE_BITMODE_24Bit;  // 24-bit mode
    NRF_TIMER1->TASKS_CLEAR     = 1;
    NRF_TIMER1->CC[0]           = 25;
    NRF_TIMER1->SHORTS          = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);


    //Enable Timer interrupt
    NRF_TIMER1->INTENCLR = 0xffffffffUL;
    NRF_TIMER1->INTENSET = (TIMER_INTENSET_COMPARE0_Set << TIMER_INTENSET_COMPARE0_Pos);
    NVIC_ClearPendingIRQ(TIMER1_IRQn);  
    NVIC_EnableIRQ(TIMER1_IRQn);



}

uint32_t timer_cycle_count = 0;
uint32_t timer_cycle_count_limit = 8000;                        // pulse length = timer_cycle_count_limit*12.5us

// timer1 interrupt handler
// toggles GPIO2 (Pin11) to generate a 40kHz waveform
void TIMER1_IRQHandler(void){

    if(NRF_TIMER1->EVENTS_COMPARE[0] == 1){
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;
    }
    nrf_gpio_pin_toggle(11);                                    // Toggle Pin11 -> GPIO2
    timer_cycle_count++;
    if(timer_cycle_count >= timer_cycle_count_limit){           // Make sure to run the pulse for only it's defined duration
        NRF_TIMER1->TASKS_STOP  = 1;
        NRF_TIMER1->TASKS_CLEAR = 1;
        timer_cycle_count       = 0;
    }
    
}

// send a 40kHz pulse who's length is determined by by timer_cycle_count_limit
void send_pulse(void){
    timer_cycle_count = 0;
    NRF_TIMER1->TASKS_START     = 1;                            // Start timer1
}

// used to abstract away the issue of networking by triggering a pulse on 
// a base node sets GPIO3 (pin12) high to trigger a pulse
// said base node starts the clock at the same time
void gpio_init_sync_test(void)
{
    nrf_gpio_cfg_input(12, NRF_GPIO_PIN_PULLDOWN);

    NVIC_EnableIRQ(GPIOTE_IRQn);                                // Enable interrupt:

    nrf_gpiote_event_config(0, 12, NRF_GPIOTE_POLARITY_LOTOHI); // Channel[0], Pin12 (GPIO3), interrupt on toggle 
}

// GPIOTE interrupt handler to detect the rising edge when testing the flight time measurement of the system
void GPIOTE_IRQHandler(void)
{
    if(NRF_GPIOTE->EVENTS_IN[0] == 1){                            // Check if GPIO3 being pulled high triggered the interrupt
        NRF_GPIOTE->EVENTS_IN[0] = 0;                             // Reset interrupt
        send_pulse();
    }
        
}
