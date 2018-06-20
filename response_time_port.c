/* HAL-only entry function */
#include "hal_data.h"
#include "response_time.h"

void response_port_init (void)
{
	ssp_err_t err;

	/** Configure switch pin as external IRQ. */
	/** OPEN EXTERNAL IRQ HERE */

	/** Open timer used for measuring response time. */
	/** OPEN TIMER HERE */

	/** Check for any errors. */
	if (err != SSP_SUCCESS)
	{
		toggle_led(BSP_LED_LED2);
		while(1)
		{
			/** Check err. */
		}
	}
}

/** Start the timer that will be used for measuring response time. */
void response_timer_start (void)
{
	/** Reset timer. */
	/** RESET TIMER USING API->RESET FUNCTION */

	/** Start timer. */
	/** START TIMER USING API->START FUNCTION */
}

/** Stop the timer that will be used for measuring response time. */
void response_timer_stop (void)
{
	/** Stop timer. */
	/** STOP TIMER USING API->STOP FUNCTION */
}

/** Get counter value which represents the number of ticks since the timer was started. */
uint32_t response_timer_counter_get (void)
{
	uint32_t temp_counter;

	/** Get counter value. */
	/** GET TIMER VALUE USING API->COUNTERGET FUNCTION. USE temp_counter TO STORE VALUE. */

	return temp_counter;
}


