// TimeToEdge.h
// Vincent Steil
// 13.11.2013


// initialise timer1 with period 1/16MHz for max resolution
void timer1_init(void);

// start timer1 measurement
void timer1_start(void);

// initialise GPIO1
void gpio_init(void);

// get ready to trigger on incoming edge
void enable_edge_trigger(void);

// disable the edge trigger
void disable_edge_trigger(void);

// rising edge interrupt
void GPIOTE_IRQHandler(void);

// call this function if you want to start measuring the time to the next edge
void start_waiting_for_edge(void);


