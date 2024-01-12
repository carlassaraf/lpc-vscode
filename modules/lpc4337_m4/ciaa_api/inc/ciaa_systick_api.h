/*
 * ciaa_systick_api.h
 *
 *  Created on: Jun 7, 2022
 *      Author: fabri
 */

#ifndef CIAA_SYSTICK_API_H_
#define CIAA_SYSTICK_API_H_

#include "chip.h"

/* Absolute time variable to track time in ms */
typedef uint32_t absolute_time_t;

/* Extern interrupt handler pointer */
extern void (*systick_handler)(void);
/* Extern absolute time variable */
extern absolute_time_t absoluteTimeMs;

/* Function prototypes */
uint8_t systick_init(uint32_t us);

/*
 * 	@brief	Starts the SysTick counter
 *
 * 	@param	None
 *
 * 	@return	None
 */
static inline void systick_start(void) { SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; }

/*
 * 	@brief	Stops the SysTick counter
 *
 * 	@param	None
 *
 * 	@param	None
 */
static inline void systick_stop(void) { SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; }

/*
 * 	@brief	Resets the SysTick counter
 *
 * 	@param	None
 *
 * 	@param	None
 */
static inline void systick_reset(void) { SysTick->VAL = 0; }

/*
 * 	@brief	Sets a new SysTick period
 *
 * 	@param	us: period in us
 *
 * 	@return	None
 */
static inline void systick_set_period_us(uint32_t us) {
	/* Stop the counter */
	systick_stop();
	/* Calculate the number of ticks to match */
	uint32_t ticks = SystemCoreClock * (us / 1E6);
	/* Configure SysTick */
	SysTick->LOAD = ticks - 1;
	/* Reset counter */
	systick_reset();
	/* Start counter */
	systick_start();
}

/*
 * 	@brief	Get the time since the SysTick was enabled in ms
 *
 * 	@param	None
 *
 * 	@return	time in ms
 */
static inline absolute_time_t systick_get_absolute_time(void) { return absoluteTimeMs; }

/*
 * 	@brief	Add a handler for the SysTick interrupt
 *
 * 	@param	handler: function pointer
 *
 * 	@return None
 */
static inline void systick_set_irq_handler(void (*handler)(void)) { systick_handler = handler; }

#endif /* CIAA_SYSTICK_API_H_ */
