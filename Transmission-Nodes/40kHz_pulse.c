// Vincent Steil
// 12.11.2013
// 40kHz_pulse.c

#include <stdint.h>
#include <stdlib.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "40kHz_pulse.h"

void OutPin_Init(void){
    nrf_gpio_cfg_output(11);                                // Set GPIO2 as output pin
    nrf_gpio_pin_write(11,1);
}

void timer1_init(void){
        
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;                    // Start 16 MHz crystal oscillator
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)             // Wait for the external oscillator to start up
    {
        // Do nothing.
    }
    
    NRF_TIMER1->TASKS_STOP      = 1;
    NRF_TIMER1->MODE            = TIMER_MODE_MODE_Timer;          // Set the timer in Timer Mode   
    NRF_TIMER1->PRESCALER       = 3;                              // Prescaler 3 produces 2Mhz Hz timer frequency => 1 tick = 32 us.  
    NRF_TIMER1->BITMODE         = TIMER_BITMODE_BITMODE_24Bit;    // 24-bit mode
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
uint32_t timer_cycle_count_limit = 8000;                    // 50ms pwm pulse

void TIMER1_IRQHandler(void){
   
    if(NRF_TIMER1->EVENTS_COMPARE[0] == 1){
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;
    }
    //NRF_TIMER1->TASKS_CLEAR = 1;
    nrf_gpio_pin_toggle(11);                                // Toggle Pin11 -> GPIO2
    timer_cycle_count++;
    if(timer_cycle_count >= timer_cycle_count_limit){       // Make sure to run the pulse for only 1ms
        NRF_TIMER1->TASKS_CLEAR = 1;
        NRF_TIMER1->TASKS_STOP  = 1;       
        timer_cycle_count       = 0;
    }
    
}

void send_pulse(void){
    timer_cycle_count = 0;
    NRF_TIMER1->TASKS_START     = 1;                            // Start timer1
}


