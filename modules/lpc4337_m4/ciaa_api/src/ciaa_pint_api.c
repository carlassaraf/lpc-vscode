/*
 * ciaa_pint_api.c
 *
 *  Created on: Dec 19, 2022
 *      Author: carlassaraf
 */

#include "ciaa_pint_api.h"

/* Pint handlers array */
void (*pint_handlers[8])(void);

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
 * @param pint_channel: PINT channel (0 - 7)
 * @param handler: function handler
 */
void pint_enable(pin_t pin, pint_mode_t mode, uint8_t pint_channel, void (*handler)(void)) {
	/* PINT interrupts */
	LPC43XX_IRQn_Type irqs[8] = {
			PIN_INT0_IRQn, PIN_INT1_IRQn, PIN_INT2_IRQn, PIN_INT3_IRQn,
			PIN_INT4_IRQn, PIN_INT5_IRQn, PIN_INT6_IRQn, PIN_INT7_IRQn
	};
	/* Map GPIO to PINT */
	scu_gpio_pint_select(pint_channel, pin.GPIO_PORT, pin.GPIO_PIN);
	/* Clear pending status */
	pint_clear_status(pint_channel);
	/* Select mode */
	pint_set_mode(pint_channel, mode);
	/* Attach interrupt */
	pint_handlers[pint_channel] = handler;
	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(irqs[pint_channel]);
	NVIC_EnableIRQ(irqs[pint_channel]);
}

/**
 * @brief Pin Interrupt Handler 0
 */
void GPIO0_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(0);
	/* If able, call handler */
	if(pint_handlers[0]) { pint_handlers[0](); }
}

/**
 * @brief Pin Interrupt Handler 1
 */
void GPIO1_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(1);
	/* If able, call handler */
	if(pint_handlers[1]) { pint_handlers[1](); }
}

/**
 * @brief Pin Interrupt Handler 2
 */
void GPIO2_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(2);
	/* If able, call handler */
	if(pint_handlers[2]) { pint_handlers[2](); }
}

/**
 * @brief Pin Interrupt Handler 3
 */
void GPIO3_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(3);
	/* If able, call handler */
	if(pint_handlers[3]) { pint_handlers[3](); }
}

/**
 * @brief Pin Interrupt Handler 4
 */
void GPIO4_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(4);
	/* If able, call handler */
	if(pint_handlers[4]) { pint_handlers[4](); }
}

/**
 * @brief Pin Interrupt Handler 5
 */
void GPIO5_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(5);
	/* If able, call handler */
	if(pint_handlers[5]) { pint_handlers[5](); }
}

/**
 * @brief Pin Interrupt Handler 6
 */
void GPIO6_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(6);
	/* If able, call handler */
	if(pint_handlers[6]) { pint_handlers[6](); }
}

/**
 * @brief Pin Interrupt Handler 7
 */
void GPIO7_IRQHandler(void) {
	/* Clear pending interrupt */
	pint_clear_status(7);
	/* If able, call handler */
	if(pint_handlers[7]) { pint_handlers[7](); }
}

