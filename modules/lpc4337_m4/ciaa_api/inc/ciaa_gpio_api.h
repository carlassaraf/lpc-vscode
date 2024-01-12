/*
 * ciaa_gpio_api.h
 *
 *  Created on: Jun 2, 2022
 *      Author: fabri
 */

#ifndef CIAA_GPIO_API_H_
#define CIAA_GPIO_API_H_

#include "chip.h"
#include "ciaa_scu_api.h"

/* Definitions */
#define CIAA_GPIO	LPC_GPIO_PORT

#define OFF		false
#define ON		true

#define LOW		false
#define HIGH	true

#define INPUT	false
#define OUTPUT	true

/**
 * @brief Pin struct definition
 */
typedef struct {
	uint8_t SCU_PORT;				/* GPIO PORT number for SCU */
	uint8_t SCU_PIN;				/* GPIO PIN number for SCU */
	uint8_t GPIO_PORT;				/* GPIO PORT to read/write */
	uint8_t GPIO_PIN;				/* GPIO PIN to read/write */
	scu_pin_functions_t *FUNCTIONS;	/* SCU functions for the PIN */
} pin_t;

/* Pin declarations */
extern pin_t LEDR;
extern pin_t LEDG;
extern pin_t LEDB;
extern pin_t LED1;
extern pin_t LED2;
extern pin_t LED3;

extern pin_t TEC1;
extern pin_t TEC2;
extern pin_t TEC3;
extern pin_t TEC4;

extern pin_t GPIO0;
extern pin_t GPIO1;
extern pin_t GPIO2;
extern pin_t GPIO3;
extern pin_t GPIO4;
extern pin_t GPIO5;
extern pin_t GPIO6;
extern pin_t GPIO7;
extern pin_t GPIO8;

extern pin_t TFIL0;
extern pin_t TFIL1;
extern pin_t TFIL2;
extern pin_t TFIL3;

extern pin_t TCOL0;
extern pin_t TCOL1;
extern pin_t TCOL2;

extern pin_t LCD1;
extern pin_t LCD2;
extern pin_t LCD3;
extern pin_t LCD4;
extern pin_t LCD_RS;
extern pin_t LCD_EN;

extern pin_t RXD1;
extern pin_t TX_EN;
extern pin_t RXD0;
extern pin_t CRS_DV;
extern pin_t MDIO;
extern pin_t TXD0;
extern pin_t TXD1;
extern pin_t MDC;

extern pin_t RS232_TXD;
extern pin_t RS232_RXD;

extern pin_t RS485_DIR;
extern pin_t RS485_TXD;
extern pin_t RS485_RXD;

extern pin_t CAN_RD;
extern pin_t CAN_TD;

extern pin_t SPI_MISO;
extern pin_t SPI_MOSI;

/* Function prototypes */

/**
 * @brief Set GPIO pin direction to input
 * @param pin: struct to the pin
 */
static void inline gpio_set_dir_in(pin_t pin) {
	/* Set pin function */
	scu_set_pin_mode(pin.SCU_PORT, pin.SCU_PIN, SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | pin.FUNCTIONS->GPIO);
	/* Set pin direction */
	CIAA_GPIO->DIR[pin.GPIO_PORT] &= ~(1UL << pin.GPIO_PIN);
}

/**
 * @brief Set GPIO pin direction to output
 * @param pin: struct to the pin
 */
static void inline gpio_set_dir_out(pin_t pin) {
	/* Set pin function */
	scu_set_pin_mode(pin.SCU_PORT, pin.SCU_PIN, SCU_MODE_INACT | SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | pin.FUNCTIONS->GPIO);
	/* Set pin direction */
	CIAA_GPIO->DIR[pin.GPIO_PORT] |= 1UL << pin.GPIO_PIN;
}

/**
 * @brief Set GPIO pin direction
 * @param pin: struct to the pin
 * @param dir: direction of the pin:
 * 		- false for input
 * 		- true for output
 */
static void inline gpio_set_dir(pin_t pin, bool dir) {
	/* Check direction and set */
	if (dir) { gpio_set_dir_out(pin); }
	else { gpio_set_dir_in(pin); }
}

/**
 * @brief Set GPIO pin output value
 * @param pin: struct to the pin to initialize
 * @param value: output value:
 * 		- false for low
 * 		- true for high
 */
static void inline gpio_put(pin_t pin, bool value) { CIAA_GPIO->B[pin.GPIO_PORT][pin.GPIO_PIN] = value; }

/**
 * @brief Set GPIO pin output value to high
 * @param pin: struct to the pin to initialize
 */
static void inline gpio_set(pin_t pin) { CIAA_GPIO->SET[pin.GPIO_PORT] = (1 << pin.GPIO_PIN); }

/**
 * @brief Set GPIO pin output value to low
 * @param pin: struct to the pin to initialize
 */
static void inline gpio_clr(pin_t pin) { CIAA_GPIO->CLR[pin.GPIO_PORT] = (1 << pin.GPIO_PIN); }

/**
 * @brief Toggle GPIO pin output value
 * @param pin: struct to the pin to initialize
 */
static void inline gpio_xor(pin_t pin) { CIAA_GPIO->NOT[pin.GPIO_PORT] = (1 << pin.GPIO_PIN); }

/**
 * @brief Get GPIO pin value
 * @param pin: struct to the pin to initialize
 * @return pin value:
 * 		- false for low
 * 		- true for high
 */
static bool inline gpio_get(pin_t pin) { return (bool) CIAA_GPIO->B[pin.GPIO_PORT][pin.GPIO_PIN]; }

#endif /* CIAA_GPIO_API_H_ */
