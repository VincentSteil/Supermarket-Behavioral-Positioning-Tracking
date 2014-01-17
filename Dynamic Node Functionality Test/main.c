// Vincent Steil
// University of Edinburgh
// testing file for the the transmission node to both test the maximum range of the system and for the flight time in the absence of networking delays


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
  timer_cycle_count++;
  gpio_init_sync_test();                                            // uncomment in order to send pulse out on the rising edge of GPIO3
  while (1) {
    if(timer_cycle_count == 0){
        send_pulse();                                               // when testing the timing function, comment this line out
        LED_counter++;
        if(LED_counter >= 10){
            nrf_gpio_pin_toggle(0);                                 // Toggle Pin02
            LED_counter = 0;
        }
    }
  }
}

