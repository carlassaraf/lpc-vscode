/*
 * ciaa_adc_api.h
 *
 *  Created on: Jun 3, 2022
 *      Author: fabri
 */

#ifndef CIAA_ADC_API_H_
#define CIAA_ADC_API_H_

#include "chip.h"

/* Base ADC register redefinitions */
#define CIAA_ADC0	LPC_ADC0
#define CIAA_ADC1	LPC_ADC1
/* CR register channels mask */
#define ADC_CHANNELS_MASK	0xff
/* Max channels for each ADC */
#define ADC_MAX_CHANNELS	8
/* Value to get a full conversion */
#define ADC_FULL_CONV_CLK	11

/**
 * @brief Number of clocks used for each conversion
 * in Burst mode
 */
typedef enum {
	adc_10bits	= ADC_10BITS,
	adc_9bits 	= ADC_9BITS,
	adc_8bits 	= ADC_8BITS,
	adc_7bits	= ADC_7BITS,
	adc_6bits	= ADC_6BITS,
	adc_5bits	= ADC_5BITS,
	adc_4bits	= ADC_4BITS,
	adc_3bits	= ADC_3BITS
} adc_bit_accuracy_t;

/**
 * @brief ADC configuration struct
 */
typedef struct {
	uint32_t rate;			/* ADC rate */
	uint8_t bitsAccuracy;	/* ADC bit accuracy */
	bool burstMode;			/* ADC Burt Mode */
	bool interrupt;			/* ADC Interrupt */
} adc_config_t;

/* Extern interrupt handler pointer */
extern void (*adc_handlers[])(void);

/* Prototype functions */
void adc_init(uint8_t adc);
void adc_config_init(uint8_t adc, adc_config_t config);
void adc_set_burst_mode_enabled(uint8_t adc, bool enabled);
void adc_select_input(uint8_t adc, uint8_t channel);
uint8_t adc_get_selected_input(uint8_t adc);
uint16_t adc_read(uint8_t adc);
uint16_t adc_get_conversion_value(uint8_t adc, uint8_t channel);
void adc_set_irq_enabled(uint8_t adc, bool enabled);

/* Inline functions */

/**
 * @brief	Get the base ADC register to work
 * @param adc: ADC number
 * @return	ADC register:
 *		- CIAA_ADC0 for ADC0
 *		- CIAA_ADC1 for ADC1
 */
static inline LPC_ADC_T* adc_get_base_reg(uint8_t adc) { return (adc)? CIAA_ADC1 : CIAA_ADC0; }

/**
 * @brief Get ADC default configuration
 * @return ADC configuration
 */
static inline adc_config_t adc_get_default_config(void) {
	/* Set up default ADC config */
	adc_config_t config = {
		ADC_MAX_SAMPLE_RATE,	/* 400 kSamples/s */
		ADC_10BITS,				/* Max resolution */
		DISABLE,				/* No burst mode */
		DISABLE					/* No interrupt */
	};
	/* Return configuration */
	return config;
}

/**
 * 	@brief Set the ADC start mode
 * 	@param adc: ADC number
 * 	@param start_mode: possible values:
 * 		- ADC_NO_START: don't start conversion
 * 		- ADC_START_NOW: start conversion now
 * 		- ADC_START_ON_CTOUT15: start when rising edge occurs on CTOUT_15
 * 		- ADC_START_ON_CTOUT8: start when rising edge occurs on CTOUT_8
 * 		- ADC_START_ON_ADCTRIG0: start when rising edge occurs on ADCTRIG0
 * 		- ADC_START_ON_ADCTRIG1: start when rising edge occurs on ADCTRIG1
 * 		- ADC_START_ON_MCOA2: start when rising edge occurs on Motocon PWM output MCOA2
 */
static inline void adc_set_start_mode(uint8_t adc, uint8_t start_mode) {
	/* Get ADC register */
	LPC_ADC_T *reg;
	reg = adc_get_base_reg(adc);
	/* Auxiliary variable */
	uint32_t temp;
	/* Clear start mode bits */
	temp = reg->CR & (~ADC_CR_START_MASK);
	/* Set new start mode with rising edge*/
	reg->CR = temp | (ADC_CR_START_MODE_SEL((uint32_t) start_mode)) | ADC_CR_EDGE;
}

/**
 * @brief Start a new conversion
 * @param adc: ADC number
 */
static inline void adc_run(uint8_t adc) { adc_set_start_mode(adc, ADC_START_NOW); }

/**
 * @brief Enable/Disable interrupt on an ADC channel
 * @param adc: ADC number
 * @param channel: ADC channel
 * @param enabled: interrupt state
 */
static inline void adc_set_irq_channel_enabled(uint8_t adc, uint8_t channel, bool enabled) {
	/* Enable/Disable interrupt in the requested channel */
	if (enabled) { adc_get_base_reg(adc)->INTEN |= (1UL << channel); }
	else { adc_get_base_reg(adc)->INTEN &= (~(1UL << channel)); }
}

/**
 * @brief Set interrupt handler for ADC
 * @param adc: ADC number
 * @param handler: function to call when interrupt occurs
 */
static inline void adc_set_irq_handler(uint8_t adc, void (*handler)(void)) { adc_handlers[adc] = handler; }

/**
 * @brief Get the actual conversion state on a channel
 * @param adc: ADC number
 * @param channel: ADC channel
 * @return conversion state. Returns true when ready, false if not ready.
 */
static inline bool adc_get_conversion_state(uint8_t adc, uint8_t channel) { return (adc_get_base_reg(adc)->STAT & (1UL << channel))? true : false; }

/**
 * @brief Get the number of clocks that a conversion takes
 * @param resolution: number of bits used in resolution
 */
static inline uint8_t adc_get_number_of_clocks(adc_bit_accuracy_t resolution) { return ADC_FULL_CONV_CLK - resolution; }

#endif /* CIAA_ADC_API_H_ */
