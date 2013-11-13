// 40kHz_pulse.h
// Vincent Steil
// 11.11.2013



// send a 80*12.5us = 0.5ms pulse at 40kHz
void send_pulse(void);


// initialise timer1 to 12.5us interrupts
static void timer1_init(void);

// timer1 interrupt handler
// toggles GPIO2 (Pin11) to generate a 40kHz waveform
// disables the timer after 0.5ms of a signal
void TIMER1_IRQHandler(void);

// configure GPIO2 (Pin11) as output pin
void OutPin_Init(void);
