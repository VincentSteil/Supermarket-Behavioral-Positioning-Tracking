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
#include "40kHz_pulse.h"




uint8_t LED_counter = 0;

int main(){
  NVIC_EnableIRQ(TIMER1_IRQn);
  __enable_irq();
  nrf_gpio_cfg_output(0);
  nrf_gpio_pin_write(0,1);

  OutPin_Init();
  timer1_init();
  send_pulse();
  while (1) {
    if(timer_cycle_count == 0){
        send_pulse();
        LED_counter++;
        if(LED_counter >= 2){
            nrf_gpio_pin_toggle(0);                                // Toggle Pin11 -> GPIO2
            LED_counter = 0;
        }
    }
  }
}

