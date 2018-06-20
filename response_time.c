/* HAL-only entry function */
#include "hal_data.h"
#include "response_time.h"

#define RESPONSE_NUM_MEASUREMENTS		(5)

static volatile bool g_timing = false;
static bool g_playing = false;
static volatile uint32_t g_delay_times_index = 0;
static uint32_t response_counts[RESPONSE_NUM_MEASUREMENTS];
static uint32_t response_counts_index = 0;

static void response_add_measurement(uint32_t latest_count);
static float response_time_get(void);

/** Callback when switch is pressed. Stops the timer measuring the response time and calculates the average. */
void switch_callback(external_irq_callback_args_t * p_args)
{
	SSP_PARAMETER_NOT_USED(p_args);

    if (g_timing == true)
	{
		/** Calculate response time. */
		response_timer_stop();
		g_timing = false;
		toggle_led(BSP_LED_LED1);
		response_add_measurement(response_timer_counter_get());
	}
}

/** Drives the response game. Runs once a second and starts the test.
 *  This is called by the GPT module. */
void game_timer_callback(timer_callback_args_t * p_args)
{
	static const uint32_t g_delay_times[] = {4, 2, 6, 3, 4};
	static uint32_t g_delay_counter = 0;

	SSP_PARAMETER_NOT_USED(p_args);

	if ((g_playing == true) && (g_timing == false))
	{
		if (g_delay_counter > g_delay_times[g_delay_times_index % ARRAY_LENGTH(g_delay_times)])
		{
			/* Turn on LED and start timer */
			toggle_led(BSP_LED_LED1);

			response_timer_start();

			/** Set flag to true to start timing. */
			g_timing = true;

			g_delay_counter = 0;
			g_delay_times_index++;
		}
		else
		{
			g_delay_counter++;
		}
	}
}

/** Initalize timer that drives the game. */
void response_init (void)
{
	ssp_err_t err;
	toggle_led(0);
	toggle_led(1);
	toggle_led(2);

	/* Initialize port */
	response_port_init();

	/* Initialize the game timer. */
	err = g_game_timer.p_api->open(g_game_timer.p_ctrl, g_game_timer.p_cfg);

	if (err != SSP_SUCCESS)
	{
		/* Error. */
		toggle_led(BSP_LED_LED2);
		while(1);
	}

	g_timing = false;
}

/** Start the game. */
float response_play (void)
{
	g_delay_times_index = 0;
	response_counts_index = 0;
	g_playing = true;

	/* Start game timer. */
	g_game_timer.p_api->reset(g_game_timer.p_ctrl);
	g_game_timer.p_api->start(g_game_timer.p_ctrl);

	while ((g_delay_times_index < RESPONSE_NUM_MEASUREMENTS) || (g_timing == true))
	{
		/* Wait for ten samples. */
	}

	/* Stop game timer. */
	g_game_timer.p_api->stop(g_game_timer.p_ctrl);
	g_playing = false;
	g_timing = false;

	/* Get average time. */
	return response_time_get();
}

/** Toggle the input LED. LEDs defined in BSP for each board. */
void toggle_led (bsp_led_t led)
{
	bsp_leds_t leds;
	ioport_level_t level;

	R_BSP_LedsGet(&leds);

	/* Toggle LED */
	g_ioport.p_api->pinRead(leds.p_leds[led], &level);

	if (IOPORT_LEVEL_HIGH == level)
	{
		level = IOPORT_LEVEL_LOW;
	} else
	{
		level = IOPORT_LEVEL_HIGH;
	}

	g_ioport.p_api->pinWrite(leds.p_leds[led], level);
}

/** Records GPT counter value. */
static void response_add_measurement (uint32_t latest_count)
{
	/** Record latest score */
	response_counts[response_counts_index] = latest_count;

	response_counts_index++;
}

/** Returns average response time in seconds (float). */
static float response_time_get (void)
{
	/** Add counts. */
	uint32_t pclka_hz;
	float total = 0;
	uint32_t i;

	/** Get PCLKA time for GPT. */
	g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKA, &pclka_hz);

	for (i = 0; i < RESPONSE_NUM_MEASUREMENTS; i++)
	{
		/** Do float conversion here to not worry about overflow of uint32_t */
		total += ((float)response_counts[i])/((float)pclka_hz);
	}

	/** Return average */
	return (total/(float)RESPONSE_NUM_MEASUREMENTS);
}

