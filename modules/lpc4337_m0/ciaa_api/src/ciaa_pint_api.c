/*
 * ciaa_pint_api.c
 *
 *  Created on: Dec 19, 2022
 *      Author: carlassaraf
 */

#include "ciaa_pint_api.h"


/* Pint handlers array */
void (*pint_handlers)(void);

/**
 * @brief Clear interrupt status on selected PINT channel
 * @param pint_channel: PINT channel number (0 - 7)
 */
static void inline pint_clear_status(uint8_t pint_channel) { LPC_GPIO_PIN_INT->IST = PININTCH(pint_channel); }

/**
 * @brief Set PINT interrupt mode to rising edge
 * @param pint_channel: PINT channel number (0 -7)
 */
static void inline pint_set_mode_rising_edge(uint8_t pint_channel) {
	/* Enable edge mode */
	LPC_GPIO_PIN_INT->ISEL &= ~PININTCH(pint_channel);
	/* Enable high mode */
	LPC_GPIO_PIN_INT->SIENR |= PININTCH(pint_channel);
}

/**
 * @brief Set PINT interrupt mode to falling edge
 * @param pint_channel: PINT channel number (0 - 7)
 */
static void inline pint_set_mode_falling_edge(uint8_t pint_channel) {
	/* Enable edge mode */
	LPC_GPIO_PIN_INT->ISEL &= ~PININTCH(pint_channel);
	/* Enable low mode */
	LPC_GPIO_PIN_INT->SIENF |= PININTCH(pint_channel);
}

/**
 * @brief Set PINT interrupt mode to level high
 * @param pint_channel: PINT channel number (0 - 7)
 */
static void inline pint_set_mode_level_high(uint8_t pint_channel) {
	/* Enable level mode */
	LPC_GPIO_PIN_INT->ISEL |= PININTCH(pint_channel);
	/* Enable high mode */
	LPC_GPIO_PIN_INT->SIENR |= PININTCH(pint_channel);
}

/**
 * @brief Set PINT interrupt mode to level low
 * @param pint_channel: PINT channel number (0 - 7)
 */
static void inline pint_set_mode_level_low(uint8_t pint_channel) {
	/* Enable level mode */
	LPC_GPIO_PIN_INT->ISEL |= PININTCH(pint_channel);
	/* Enable low mode */
	LPC_GPIO_PIN_INT->SIENF |= PININTCH(pint_channel);
}

/**
 * @brief Choose PINT mode
 * @param pint_channel: PINT channel (0 - 7)
 * @param mode: interrupt trigger type (rising_edge, falling_edge, high_level, low_level)
 */
static void inline pint_set_mode(uint8_t pint_channel, pint_mode_t mode) {
	/* Check mode and enable */
	if(mode == rising_edge) { pint_set_mode_rising_edge(pint_channel); }
	else if(mode == falling_edge) { pint_set_mode_falling_edge(pint_channel); }
	else if(mode == high_level) { pint_set_mode_level_high(pint_channel); }
	else { pint_set_mode_level_low(pint_channel); }
}

/**
 * @brief Enable pin interrupt for a specified pin
 * @param mode: interrupt trigger type (rising_edge, falling_edge, high_level, low_level)
 * @param handler: function handler
 */
void pint_enable(pin_t pin, pint_mode_t mode, void (*handler)(void)) {
	/* Map GPIO to PINT */
	scu_gpio_pint_select(4, pin.GPIO_PORT, pin.GPIO_PIN);
	/* Clear pending status */
	pint_clear_status(4);
	/* Select mode */
	pint_set_mode(4, mode);
	/* Attach interrupt */
	pint_handlers = handler;
	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(PIN_INT4_IRQn);
	NVIC_EnableIRQ(PIN_INT4_IRQn);
}

/**
 * @brief Pin Interrupt Handler 4
 */
void GPIO4_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(4);
	/* If able, call handler */
	if(pint_handlers) { pint_handlers(); }
}
