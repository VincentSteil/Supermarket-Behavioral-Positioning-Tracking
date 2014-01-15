#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "math.h"
#include "simple_uart.h"
#include "nrf_gpiote.h"
#include "TimeToEdge.h"

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





int main(){
  uint32_t LED_counter = 0;
  NVIC_EnableIRQ(GPIOTE_IRQn);
  __enable_irq();

  nrf_gpio_cfg_output(0); 
  gpio_init();
  timer1_init();
  enable_edge_trigger();
  
  while (1) {
    LED_counter++;
    if (LED_counter > 500000){
        LED_counter = 0;
        nrf_gpio_pin_clear(0);
        enable_edge_trigger();
    }
  }
}
