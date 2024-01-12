/*
 * ciaa_scu_api.h
 *
 *  Created on: Jun 6, 2022
 *      Author: fabri
 */

#ifndef CIAA_SCU_API_H_
#define CIAA_SCU_API_H_

#include "chip.h"

#define CIAA_SCU	LPC_SCU

#define SCU_MODE_PULLUP            (0x0 << 3)		/* Enable pull-up resistor at pad */
#define SCU_MODE_REPEATER          (0x1 << 3)		/* Enable pull-down and pull-up resistor at resistor at pad (repeater mode) */
#define SCU_MODE_INACT             (0x2 << 3)		/* Disable pull-down and pull-up resistor at resistor at pad */
#define SCU_MODE_PULLDOWN          (0x3 << 3)		/* Enable pull-down resistor at pad */
#define SCU_MODE_HIGHSPEEDSLEW_EN  (0x1 << 5)		/* Enable high-speed slew */
#define SCU_MODE_INBUFF_EN         (0x1 << 6)		/* Enable Input buffer */
#define SCU_MODE_ZIF_DIS           (0x1 << 7)		/* Disable input glitch filter */
#define SCU_MODE_4MA_DRIVESTR      (0x0 << 8)		/* Normal drive: 4mA drive strength */
#define SCU_MODE_8MA_DRIVESTR      (0x1 << 8)		/* Medium drive: 8mA drive strength */
#define SCU_MODE_14MA_DRIVESTR     (0x2 << 8)		/* High drive: 14mA drive strength */
#define SCU_MODE_20MA_DRIVESTR     (0x3 << 8)		/* Ultra high- drive: 20mA drive strength */
#define SCU_MODE_FUNC0             0x0				/* Selects pin function 0 */
#define SCU_MODE_FUNC1             0x1				/* Selects pin function 1 */
#define SCU_MODE_FUNC2             0x2				/* Selects pin function 2 */
#define SCU_MODE_FUNC3             0x3				/* Selects pin function 3 */
#define SCU_MODE_FUNC4             0x4				/* Selects pin function 4 */
#define SCU_MODE_FUNC5             0x5				/* Selects pin function 5 */
#define SCU_MODE_FUNC6             0x6				/* Selects pin function 6 */
#define SCU_MODE_FUNC7             0x7				/* Selects pin function 7 */
#define SCU_PINIO_FAST             (SCU_MODE_INACT | SCU_MODE_HIGHSPEEDSLEW_EN | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS)
/* Pin configuration for STANDARD/FAST mode I2C */
#define I2C0_STANDARD_FAST_MODE		(1 << 3 | 1 << 11)
/* Pin configuration for Fast-mode Plus I2C */
#define I2C0_FAST_MODE_PLUS			(2 << 1 | 1 << 3 | 1 << 7 | 1 << 10 | 1 << 11)

/* SCU pin functions struct */
typedef struct {
	uint8_t GPIO;
	uint8_t CTOUT;
	uint8_t USART;
	uint8_t I2C;
	uint8_t SSP;
	uint8_t I2S;
	uint8_t USB;
	uint8_t CAN;
} scu_pin_functions_t;

/* Extern pins with their functions */
extern scu_pin_functions_t P2_0;
extern scu_pin_functions_t P2_1;
extern scu_pin_functions_t P2_2;
extern scu_pin_functions_t P2_10;
extern scu_pin_functions_t P2_11;
extern scu_pin_functions_t P2_12;

extern scu_pin_functions_t P1_0;
extern scu_pin_functions_t P1_1;
extern scu_pin_functions_t P1_2;
extern scu_pin_functions_t P1_6;

extern scu_pin_functions_t P6_1;
extern scu_pin_functions_t P6_4;
extern scu_pin_functions_t P6_5;
extern scu_pin_functions_t P6_7;
extern scu_pin_functions_t P6_8;
extern scu_pin_functions_t P6_9;
extern scu_pin_functions_t P6_10;
extern scu_pin_functions_t P6_11;
extern scu_pin_functions_t P6_12;

extern scu_pin_functions_t P4_0;
extern scu_pin_functions_t P4_1;
extern scu_pin_functions_t P4_2;
extern scu_pin_functions_t P4_3;
extern scu_pin_functions_t P1_5;
extern scu_pin_functions_t P7_4;
extern scu_pin_functions_t P7_5;

extern scu_pin_functions_t P4_4;
extern scu_pin_functions_t P4_5;
extern scu_pin_functions_t P4_6;
extern scu_pin_functions_t P4_10;
extern scu_pin_functions_t P4_8;
extern scu_pin_functions_t P4_9;

extern scu_pin_functions_t P0_0;
extern scu_pin_functions_t P0_1;
extern scu_pin_functions_t P1_15;
extern scu_pin_functions_t P1_16;
extern scu_pin_functions_t P1_17;
extern scu_pin_functions_t P1_18;
extern scu_pin_functions_t P1_19;
extern scu_pin_functions_t P1_20;
extern scu_pin_functions_t P7_7;

extern scu_pin_functions_t P2_3;
extern scu_pin_functions_t P2_4;

extern scu_pin_functions_t P6_2;
extern scu_pin_functions_t P9_5;
extern scu_pin_functions_t P9_6;

extern scu_pin_functions_t P3_1;
extern scu_pin_functions_t P3_2;

extern scu_pin_functions_t PF_4;
extern scu_pin_functions_t P1_3;
extern scu_pin_functions_t P1_4;

extern scu_pin_functions_t P6_3;
extern scu_pin_functions_t P6_6;

/**
 * @brief Sets IO control pin mux
 * @param scu_port: SCU PORT of the GPIO
 * @param scu_pin: SCU PIN of the GPIO
 * @param mode: function to set to the pin
 */
static inline void scu_set_pin_mode(uint8_t scu_port,  uint8_t scu_pin, uint16_t mode) { CIAA_SCU->SFSP[scu_port][scu_pin] = mode; }

/**
 * @brief Selects external interrupt for GPIO pin
 * @param pint_channel: PINT channel (0 to 7)
 * @param scu_port: GPIO SCU port
 * @param scu_pin: GPIO SCU pin
 */
static inline void scu_gpio_pint_select(uint8_t pint_channel, uint8_t scu_port, uint8_t scu_pin) {
	/* Get values to map PINT channel to GPIO */
	int32_t of = (pint_channel & 3) << 3;
	uint32_t val = (((scu_port & 0x7) << 5) | (scu_pin & 0x1F)) << of;
	LPC_SCU->PINTSEL[pint_channel >> 2] = (LPC_SCU->PINTSEL[pint_channel >> 2] & ~(0xFF << of)) | val;
}

/**
 * @brief I2C0 configuration
 * @param mode: I2C0 mode. Should be:
 * 		- I2C0_STANDARD_FAST_MODE
 *		- I2C0_FAST_MODE_PLUS
 */
static inline void scu_i2c0_config(uint32_t mode) { CIAA_SCU->SFSI2C0 = mode; }

/**
 * @brief Configure ADC channel
 * @param adc: ADC number
 * @param channel: one of the 8 available ADC channels
 */
static inline void scu_adc_channel_config(uint8_t adc, uint8_t channel) { CIAA_SCU->ENAIO[adc] |= 1UL << channel; }

/**
 * @brief Enable/Disable DAC output on P4_4
 * @param enabled: whether to enable or disable the DAC output
 */
static inline void scu_dac_config(bool enabled) {
	/* Clear ENAIO bit that enables P4_4 DAC output */
	CIAA_SCU->ENAIO[2] &= ~1;
	/* Set bit if the output in P4_4 is required */
	if(enabled) { CIAA_SCU->ENAIO[2] |= 1; }
}

#endif /* CIAA_SCU_API_H_ */
