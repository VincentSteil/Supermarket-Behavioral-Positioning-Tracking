#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "math.h"
#include "simple_uart.h"
#include "nrf_gpiote.h"

// Debug helper variables
static volatile bool init_ok, enable_ok, push_ok, pop_ok, tx_success;

/**@brief Error handler function, which is called when an error has occurred. 
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze 
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
 
    // This call can be used for debug purposes during development of an application.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}


/** @brief Function for initializing Timer 0.
 */
static void timer1_init(void)
{
        
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;                    // Start 16 MHz crystal oscillator
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)             // Wait for the external oscillator to start up
    {
        // Do nothing.
    }

    
    NRF_TIMER1->MODE      = TIMER_MODE_MODE_Timer;          // Set the timer in Timer Mode
    
    NRF_TIMER1->PRESCALER = 0;                              // Set timer freq to 16 MHz / 2^0 -> 0.0625us
    
    NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit;      // 32-bit mode

}

void timer1_start(void){
    NRF_TIMER1->TASKS_CLEAR = 1;                            // Clear timer1
    NRF_TIMER1->TASKS_START = 1;                            // Start timer1
}

static void gpio_init(void)
{
    nrf_gpio_cfg_input(11, NRF_GPIO_PIN_NOPULL);            // Enable Pin11 -> GPIO2

    NVIC_EnableIRQ(GPIOTE_IRQn);                            // Enable interrupt:
    
    NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos)      // Set interrupt to trigger on rising edge
                           | (11 << GPIOTE_CONFIG_PSEL_Pos)                                     // Set interrupt to trigger on Pin11 -> GPIO2
                           | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos); 
}

void enable_edge_trigger(void){  
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;                 // Enable the edge triggered interrupt on GPIO2
}

void disable_edge_trigger(void){  
    NRF_GPIOTE->INTENCLR  = GPIOTE_INTENCLR_IN0_Clear << GPIOTE_INTENCLR_IN0_Pos;                 // Disable the edge triggered interrupt on GPIO2
}

void GPIOTE_IRQHandler(void)
{
//    unsigned char buf[32];
    nrf_gpio_pin_toggle(0);
    uint32_t timer_delay;
    
    NRF_TIMER1->TASKS_CAPTURE[0]  = 1;                            // Write time delay to CC[0] register
    NRF_TIMER1->TASKS_STOP        = 1;                            // Stop Timer1 
    timer_delay                   = NRF_TIMER1->CC[0];            // Copy CC[0] register to timer_delay
//    sprintf((char*)buf, "timer_delay: %u\n", timer_delay);
//    simple_uart_putstring(buf);
    disable_edge_trigger();
    // TRIGGER FUNTION TO SEND OUT DATA  
    
    if ((NRF_GPIOTE->EVENTS_IN[0] == 1) &&                  // Check if GPIO2 being pulled high triggered the interrupt
        (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))
    {
        NRF_GPIOTE->EVENTS_IN[0] = 0;                       // Reset interrupt
    }

//    timer1_start();
}

void start_waiting_for_edge(void){
    timer1_start();
    enable_edge_trigger();
}


int main(){

  NVIC_EnableIRQ(GPIOTE_IRQn);
  __enable_irq();

//  nrf_gpio_cfg_output(12);
//  nrf_gpio_pin_write(12, 1);
//  simple_uart_config(0, 23, 0, 22, 0);
  nrf_gpio_cfg_output(0);

  
  gpio_init();
  timer1_init();
  enable_edge_trigger();
  
  while (1) {

  }
}
