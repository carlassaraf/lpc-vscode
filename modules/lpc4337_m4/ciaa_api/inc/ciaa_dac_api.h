/*
 * ciaa_dac_api.h
 *
 *  Created on: Jun 4, 2022
 *      Author: fabri
 */

#ifndef CIAA_DAC_API_H_
#define CIAA_DAC_API_H_

#include "chip.h"
#include "ciaa_scu_api.h"

/* Base DAC register redefinition */
#define CIAA_DAC	LPC_DAC
/* Bit required to be set in the CTRL register to enable output */
#define DMA_ENA_BIT	3
/* Redefinition of LPC Open enum */
typedef DAC_CURRENT_OPT_T dac_bias_t;
/*
 * Possible bias values:
 * 	- DAC_MAX_UPDATE_RATE_400kHz
 *  - DAC_MAX_UPDATE_RATE_1MHz
 */

/* DAC config struct */
typedef struct {
	bool enabled;				/* Enable/Disable DAC output */
	uint16_t value;				/* Startint output valut */
	dac_bias_t bias;			/* Current bias for update rate */
	bool alternativeOutput;		/* Enable output on P4_4 */
} dac_config_t;

/* Function prototypes */
void dac_init(void);
void dac_config_init(dac_config_t config);

/* Inline functions */

/*
 *	@brief	Gets default DAC configuration
 * 	
 * 	@param	None
 * 	
 * 	@return DAC configuration 
 */
static inline dac_config_t dac_get_default_config(void) {
	/* Set up default configuration struct */
	dac_config_t config = {
		true,						/* DAC output enabled */
		0x200,						/* DAC output to half */
		DAC_MAX_UPDATE_RATE_1MHz,	/* Bias for max update rate */
		false						/* Output on default pin */
	};
	return config;
}

/*
 *	@brief	Set DAC current bias
 *
 *	@param	bias: DAC current bias
 * 
 * 	@return	None
 */ 
static inline void dac_set_bias(dac_bias_t bias) {
	/* Clear bias bit */
	CIAA_DAC->CR &= ~DAC_BIAS_EN;
	/* Set bit if 400kHz update rate */
	if (bias == DAC_MAX_UPDATE_RATE_400kHz) { CIAA_DAC->CR |= DAC_BIAS_EN; }
}

/*
 *	@brief	Enable/Disable DAC output
 * 
 * 	@param	enabled: output state	
 * 
 *	@return	None
 */ 
static inline void dac_set_enabled(bool enabled) {
	/* Clear DMA Enable bit */
	CIAA_DAC->CTRL &= ~(1 << DMA_ENA_BIT);
	/* Enable if output is enabled */
	if(enabled) { CIAA_DAC->CTRL |= 1 << DMA_ENA_BIT; }
}

/*
 *	@brief	Set DAC output value
 * 
 * 	@param	output: 10-bit value for the output	
 * 
 *	@return	None
 */ 
static inline void dac_set_output_value(uint16_t output) {
	/* Auxiliary variable */
	uint32_t tmp;
	/* Get current bias */
	tmp = CIAA_DAC->CR & DAC_BIAS_EN;
	/* Add desired value */
	tmp |= DAC_VALUE((output & 0x3ff));
	/* Update value */
	CIAA_DAC->CR = tmp;
}

/*
 *	@brief	Enable DAC output on P4_4 
 * 
 * 	@param	enabled: P4_4 DAC state	
 * 
 *	@return	None
 */ 
static inline void dac_set_alternative_output_enabled(bool enabled) { scu_dac_config(enabled); }

#endif /* CIAA_DAC_API_H_ */
