/*
 * ciaa_pint_api.h
 *
 *  Created on: Dec 19, 2022
 *      Author: carlassaraf
 */

#ifndef CIAA_PINT_API_H_
#define CIAA_PINT_API_H_

#include "ciaa_gpio_api.h"
#include "ciaa_scu_api.h"

/**
 * @brief Pin interrupt modes
 */
typedef enum {
	rising_edge = 0x00,				/* Rising edge */
	falling_edge = 0x01,			/* Falling edge */
	high_level = 0x02,				/* High level */
	low_level = 0x03				/* Low level */
} pint_mode_t;

/* Function prototypes */
void pint_enable(pin_t pin, pint_mode_t mode, void (*handler)(void));

#endif /* CIAA_PINT_API_H_ */
