/*
 * ciaa_stdlib.c
 *
 *  Created on: Jun 8, 2022
 *      Author: fabri
 */

#include "ciaa_stdlib.h"

/* Extern sleep end flag */
bool sleep_flags[] = { false, false, false, false };

/*
 * 	@brief	Delay for a number of milliseconds
 *
 * 	@param	ms: number of milliseconds
 *
 * 	@return	None
 */
void sleep_ms(uint32_t ms) {
	/* Get the number of ticks necessary */
	uint32_t ticks = timer_ms_to_ticks(ms);
	/* Sleep said number of ticks */
	sleep_ticks(ticks);
}

/*
 * 	@brief	Delay for a number of microseconds
 *
 * 	@param	us: number of microseconds
 *
 * 	@return	None
 */
void sleep_us(uint32_t us) {
	/* Get the number of ticks necessary */
	uint32_t ticks = timer_us_to_ticks(us);
	/* Sleep said number of ticks */
	sleep_ticks(ticks);
}

/*
 * 	@brief	Delay for a number of ticks
 *
 * 	@param	ticks: number of ticks
 *
 * 	@return	None
 */
void sleep_ticks(uint32_t ticks) {
	/* Initialize TIMER */
	timer_init(DELAY_TIMER);
	/* Reset counter value */
	timer_reset(DELAY_TIMER);
	/* Set a new match to trigger interrupt */
	timer_set_match_ticks(DELAY_TIMER, ticks);
	/* Enable TIMER0 interrupt */
	timer_set_irq_enabled(DELAY_TIMER, true);
	/* Set a handler for the interrupt */
	timer_set_irq_handler(DELAY_TIMER, sleep_set_flag);
	/* Start TIMER0 counter */
	timer_start(DELAY_TIMER);
	/* Wait for interrupt to trigger */
	while(!sleep_get_flag(DELAY_TIMER));
	/* Clear sleep flag */
	sleep_clear_flag(DELAY_TIMER);
	/* Stop TIMER conter */
	timer_stop(DELAY_TIMER);
	/* Reset counter value */
	timer_reset(DELAY_TIMER);
	/* Disable TIMER */
	timer_deinit(DELAY_TIMER);
}
