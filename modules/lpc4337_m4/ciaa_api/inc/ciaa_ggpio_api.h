/*
 * ciaa_ggpio_api.h
 *
 *  Created on: Dec 19, 2022
 *      Author: carlassaraf
 */

#ifndef CIAA_GGPIO_API_H_
#define CIAA_GGPIO_API_H_

#include "ciaa_gpio_api.h"

/**
 * @brief Group GPIO modes
 */
typedef enum {
	and_mode = 0x00,				/* AND mode */
	or_mode	= 0x01					/* OR mode */
} ggpio_mode_t;

/**
 * @brief Group GPIO interrupt modes
 */
typedef enum {
	rising_edge = 0x00,				/* Rising edge */
	falling_edge = 0x01,			/* Falling edge */
	high_level = 0x02,				/* High level */
	low_level = 0x03				/* Low level */
} ggpio_trigger_t;

/* Function prototypes */
void ggpio_enable(uint8_t gpio_group, ggpio_mode_t mode, void (*handler)(void));
void ggpio_attach_pin(uint8_t gpio_group, pin_t pin, ggpio_trigger_t trigger);

#endif /* CIAA_GGPIO_API_H_ */
