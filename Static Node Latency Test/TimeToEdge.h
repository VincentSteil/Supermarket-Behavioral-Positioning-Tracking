#ifndef TIME_TO_EDGE_H
#define TIME_TO_EDGE_H

// TimeToEdge.h
// Vincent Steil
// 13.11.2013

// if nonzero the timer_delay is ready to be read (clear it after reading)
extern volatile char timer_delay_ready;

// value of the last TOF measurement
extern volatile uint32_t timer_delay;

// initialise timer1 with period 1/16MHz for max resolution
void timer1_init(void);

// start timer1 measurement
//static void timer1_start(void); - it's static (private to module), other modules don't need to know about it 

// initialise GPIO1
void gpio_init(void);

// get ready to trigger on incoming edge
//static void enable_edge_trigger(void); - static

// disable the edge trigger
//static void disable_edge_trigger(void); - static

// rising edge interrupt
void GPIOTE_IRQHandler(void);

// call this function if you want to start measuring the time to the next edge
void start_waiting_for_edge(void);

//call this function to handle GPIOTE event on channel 0
void tte_gpioteIrqHandler();

#endif //TIME_TO_EDGE_H
