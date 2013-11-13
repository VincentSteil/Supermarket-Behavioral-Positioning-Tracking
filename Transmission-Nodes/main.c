/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup nrf_dev_timer_example_main main.c
 * @{
 * @ingroup nrf_dev_timer_example
 * @brief Timer Example Application main file.
 *
 * This file contains the source code for a sample application using Timer0, Timer1 and Timer2.
 *
 * @image html example_board_setup_a.jpg "Use board setup A for this example."
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "simple_uart.h"
#include "nrf_gpiote.h"




static void timer1_init(void){
        
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;                    // Start 16 MHz crystal oscillator
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)             // Wait for the external oscillator to start up
    {
        // Do nothing.
    }
    
    NRF_TIMER1->TASKS_STOP      = 1;
    NRF_TIMER1->MODE            = TIMER_MODE_MODE_Timer;          // Set the timer in Timer Mode   
    NRF_TIMER1->PRESCALER       = 3;                              // Prescaler 2 produces 2Mhz Hz timer frequency => 1 tick = 32 us.  
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

uint8_t timer_cycle_count = 0;
uint8_t timer_cycle_count_limit = 80;                       // 1ms pwm pulse

void TIMER1_IRQHandler(void){

    if(NRF_TIMER1->EVENTS_COMPARE[0] == 1){
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;
    }
    nrf_gpio_pin_toggle(11);                                // Toggle Pin11 -> GPIO2
    timer_cycle_count++;
    if(timer_cycle_count >= timer_cycle_count_limit){       // Make sure to run the pulse for only 1ms
        NRF_TIMER1->TASKS_STOP  = 1;
        NRF_TIMER1->TASKS_CLEAR = 1;
        timer_cycle_count       = 0;
    }
    
}

void send_pulse(void){
    timer_cycle_count = 0;
    NRF_TIMER1->TASKS_START     = 1;                            // Start timer1
}


uint8_t LED_counter = 0;

int main(){
  NVIC_EnableIRQ(TIMER1_IRQn);
  __enable_irq();

//  nrf_gpio_cfg_output(12);
//  nrf_gpio_pin_write(12, 1);
//  simple_uart_config(0, 23, 0, 22, 0);
  nrf_gpio_cfg_output(0);
  nrf_gpio_pin_write(0,1);
  nrf_gpio_cfg_output(11);

  
  timer1_init();
  send_pulse();
  while (1) {
    if(timer_cycle_count == 0){
        send_pulse();
        LED_counter++;
        if(LED_counter >= 100){
            nrf_gpio_pin_toggle(0);                                // Toggle Pin11 -> GPIO2
            LED_counter = 0;
        }
    }
  }
}

