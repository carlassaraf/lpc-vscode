/*
 * ciaa_ggpio_api.c
 *
 *  Created on: Dec 19, 2022
 *      Author: carlassaraf
 */

#include "ciaa_ggpio_api.h"

/* Group GPIO handlers array */
void (*ggpio_handlers[2])(void);

/**
 * @brief Clear group GPIO interrupt status
 * @param gpio_group: number of GPIO group (0 - 1)
 */
static void inline ggpio_clear_status(uint8_t gpio_group) { LPC_GPIOGROUP[gpio_group].CTRL |= GPIOGR_INT; }

/**
 * @brief Enable rising edge mode for GPIO group pin
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param pin: GPIO pin to use
 */
static void inline ggpio_set_mode_rising_edge(uint8_t gpio_group, pin_t pin) {
	/* Set edge mode */
	LPC_GPIOGROUP[gpio_group].CTRL &= ~GPIOGR_TRIG;
	/* Set rising edge */
	LPC_GPIOGROUP[gpio_group].PORT_POL[pin.GPIO_PORT] |= 1 << pin.GPIO_PIN;
}

/**
 * @brief Enable falling edge mode for GPIO group pin
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param pin: GPIO pin to use
 */
static void inline ggpio_set_mode_falling_edge(uint8_t gpio_group, pin_t pin) {
	/* Set edge mode */
	LPC_GPIOGROUP[gpio_group].CTRL &= ~GPIOGR_TRIG;
	/* Set falling edge */
	LPC_GPIOGROUP[gpio_group].PORT_POL[pin.GPIO_PORT] &= ~(1 << pin.GPIO_PIN);
}

/**
 * @brief Enable high level mode for GPIO group pin
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param pin: GPIO pin to use
 */
static void inline ggpio_set_mode_level_high(uint8_t gpio_group, pin_t pin) {
	/* Set level mode */
	LPC_GPIOGROUP[gpio_group].CTRL |= GPIOGR_TRIG;
	/* Set rising edge */
	LPC_GPIOGROUP[gpio_group].PORT_POL[pin.GPIO_PORT] |= 1 << pin.GPIO_PIN;
}

/**
 * @brief Enable low level mode for GPIO group pin
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param pin: GPIO pin to use
 */
static void inline ggpio_set_mode_level_low(uint8_t gpio_group, pin_t pin) {
	/* Set level mode */
	LPC_GPIOGROUP[gpio_group].CTRL |= GPIOGR_TRIG;
	/* Set falling edge */
	LPC_GPIOGROUP[gpio_group].PORT_POL[pin.GPIO_PORT] &= ~(1 << pin.GPIO_PIN);
}

/**
 * @brief Set group GPIO interrupt mode
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param mode: interrupt mode (rising_edge, falling_edge, level_high, level_low)
 * @param pin: pin to use
 */
static void inline ggpio_set_mode(uint8_t gpio_group, ggpio_trigger_t mode, pin_t pin) {
	/* Check mode and enable */
	if(mode == rising_edge) { ggpio_set_mode_rising_edge(gpio_group, pin); }
	else if(mode == falling_edge) { ggpio_set_mode_falling_edge(gpio_group, pin); }
	else if(mode == high_level) { ggpio_set_mode_level_high(gpio_group, pin); }
	else { ggpio_set_mode_level_low(gpio_group, pin); }
}

/**
 * @brief Select group GPIO AND mode
 * @param gpio_group: number of GPIO group (0 - 1)
 */
static void inline ggpio_set_and_mode(uint8_t gpio_group) { LPC_GPIOGROUP[gpio_group].CTRL |= GPIOGR_COMB; }

/**
 * @brief Select group GPIO OR mode
 * @param gpio_group: number of GPIO group (0 - 1)
 */
static void inline ggpio_set_or_mode(uint8_t gpio_group) { LPC_GPIOGROUP[gpio_group].CTRL &= ~GPIOGR_COMB; }

/**
 * @brief Enable GPIO interrupt for a pin
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param mode: interrupt mode (AND, OR)
 * @param handler: pointer to function handler
 */
void ggpio_enable(uint8_t gpio_group, ggpio_mode_t mode, void (*handler)(void)) {
	/* Group GPIO interrupts */
	LPC43XX_IRQn_Type irqs[2] = { GINT0_IRQn, GINT1_IRQn };
	/* Clear any previous interrupt */
	ggpio_clear_status(gpio_group);
	/* Register callback */
	ggpio_handlers[gpio_group] = handler;
	/* Choose mode */
	if(mode) { ggpio_set_or_mode(gpio_group); }
	else { ggpio_set_and_mode(gpio_group); }
	/* Clear and enable interrupt */
	NVIC_ClearPendingIRQ(irqs[gpio_group]);
	NVIC_EnableIRQ(irqs[gpio_group]);
}

/**
 * @brief Attach a pin to the group interrupt
 * @param gpio_group: number of GPIO group (0 - 1)
 * @param pin: pin to attach
 * @param trigger: interrupt trigger (rising_edge, falling_edge, level_high, level_low)
 */
void ggpio_attach_pin(uint8_t gpio_group, pin_t pin, ggpio_trigger_t trigger) {
	/* Set mode */
	ggpio_set_mode(gpio_group, trigger, pin);
	/* Enable group pins */
	LPC_GPIOGROUP[gpio_group].PORT_ENA[pin.GPIO_PORT] |= 1 << pin.GPIO_PIN;
}

/**
 * @brief GPIO group interrupt 0
 */
void GINT0_IRQHandler(void) {
	/* Clear pending interrupt */
	ggpio_clear_status(0);
	/* If able, call handler */
	if(ggpio_handlers[0]) { ggpio_handlers[0](); }
}

/**
 * @brief GPIO group interrupt 1
 */
void GINT1_IRQHandler(void) {
	/* Clear pending interrupt */
	ggpio_clear_status(1);
	/* If able, call handler */
	if(ggpio_handlers[1]) { ggpio_handlers[1](); }
}
