// 40kHz_pulse.h
// Vincent Steil




// send a 40kHz pulse who's length is determined by by timer_cycle_count_limit
void send_pulse(void);


// initialise timer1 to 12.5us interrupts
void timer1_init(void);

// timer1 interrupt handler
// toggles GPIO2 (Pin11) to generate a 40kHz waveform
void TIMER1_IRQHandler(void);

// configure GPIO2 (Pin11) as output pin
void OutPin_Init(void);

// used to keep track of the pulse length
// made extern to use in testing main.c file to chain pulses together to form constant 40kHz waveform
extern uint32_t timer_cycle_count;

// used to abstract away the issue of networking by triggering a pulse on 
// a base node sets GPIO3 (pin12) high to trigger a pulse
// said base node starts the clock at the same time
void gpio_init_sync_test(void);

// GPIOTE interrupt handler to detect the rising edge when testing the flight time measurement of the system
void GPIOTE_IRQHandler(void);