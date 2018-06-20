/* HAL-only entry function */
#include "hal_data.h"

/** Functions related to getting the response time. */
void response_init(void);
void response_port_init(void);
void response_timer_start(void);
void response_timer_stop(void);
uint32_t response_timer_counter_get(void);
float response_play(void);

void toggle_led(bsp_led_t pin);

/** Gets the length of an array. */
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))
