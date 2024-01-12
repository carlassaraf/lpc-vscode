/*
 * ciaa_adc_api.c
 *
 *  Created on: Jun 3, 2022
 *      Author: fabri
 */

#include "ciaa_adc_api.h"

/* Available channels */
ADC_CHANNEL_T channels[] = {
	ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3,
	ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7
};

/* Interrupt handler function pointer */
void (*adc_handlers[])(void) = { NULL, NULL };

/**
 * @brief Initializes ADC with default config
 * @param adc: ADC number
 */
void adc_init(uint8_t adc) {
	/* Default config */
	adc_config_t config = adc_get_default_config();
	/* Initialize ADC */
	adc_config_init(adc, config);
}

/**
 * @brief Initializes ADC with given config
 * @param adc: ADC number
 * @param config: ADC configuration struct
 */
void adc_config_init(uint8_t adc, adc_config_t config) {
	/* Auxiliary variables to calculate register values */
	uint8_t div;
	uint32_t cr = 0;
	uint32_t clk;
	LPC_ADC_T *reg;
	uint32_t adcBlockFreq;
	uint32_t fullAdcRate;
	/* Get ADC register */
	reg = adc_get_base_reg(adc);
	/* Get clock index */
	CHIP_CCU_CLK_T clkADC = (adc)? CLK_APB3_ADC1 : CLK_APB3_ADC0;
	/* Enable peripherial clock */
	Chip_Clock_EnableOpts(clkADC, true, true, 1);
	/* Disable all interrupts */
	reg->INTEN = 0;
	/* Enable ADC */
	cr |= ADC_CR_PDN;
	/* Click for full conversion */
	clk = adc_get_number_of_clocks(config.bitsAccuracy);
	/* The APB clock (PCLK_ADC0) is divided by (CLKDIV+1) to produce the clock for
	 * A/D converter, which should be less than or equal to 4.5MHz.
	 * A fully conversion requires (bits_accuracy+1) of these clocks.
	 * ADC Clock = PCLK_ADC0 / (CLKDIV + 1);
	 * ADC rate = ADC clock / (the number of clocks required for each conversion);
	 */
	adcBlockFreq = Chip_Clock_GetRate(clkADC);
	/* Get the round value */
	div = adcBlockFreq / (config.rate * clk) - 1;
	/* Clock divider */
	cr |= ADC_CR_CLKDIV(div);
	/* Number of ADC accuracy bits */
	cr |= ADC_CR_BITACC(config.bitsAccuracy);
	/* Set register value */
	reg->CR = cr;
	/* Enable/disable burst mode */
	adc_set_burst_mode_enabled(adc, config.burstMode);
	/* Enable/disable interrupt */
	adc_set_irq_enabled(adc, config.interrupt);
}

/**
 * @brief Enable burst mode
 *
 * @param adc: ADC number
 * @param enabled: burst mode state
 */
void adc_set_burst_mode_enabled(uint8_t adc, bool enabled) {
	/* Get ADC register */
	LPC_ADC_T *reg;
	reg = adc_get_base_reg(adc);
	/* Required for burst mode to trigger */
	adc_set_start_mode(adc, ADC_NO_START);
	/* Enable/Disable burst mode */
    if (enabled) { reg->CR |= ADC_CR_BURST; }
	else { reg->CR &= ~ADC_CR_BURST; }
}

/**
 * @brief Selects a single ADC channel
 * @param adc: ADC number
 * @param channel: ADC channel
 */
void adc_select_input(uint8_t adc, uint8_t channel) {
	/* Get ADC register */
	LPC_ADC_T *reg;
	reg = adc_get_base_reg(adc);
	/* Disable all channels */
	reg->CR &= ~0xff;
	/* Enable desired channel */
	reg->CR |= 1UL << channel;
}

/**
 * @brief Get single selected input
 * @param adc: ADC number
 * @return selected ADC channel
 */
uint8_t adc_get_selected_input(uint8_t adc) {
	/* Get ADC register */
	LPC_ADC_T *reg;
	reg = adc_get_base_reg(adc);
	/* Get and return the selected channel */
	for(uint8_t channel = 0; channel < ADC_MAX_CHANNELS; channel++) {
		bool match = (reg->CR & (1UL << channel));
		if(match) { return channel; }
	}
	return 0xff;
}

/**
 * @brief Returns ADC conversion value. If no interrupt is enabled,
 * it also starts the conversion and waits for it to finish
 * @param adc: ADC number
 * @return ADC conversion value
 */
uint16_t adc_read(uint8_t adc) {
	/* ADC selected value */
	ADC_CHANNEL_T channel = (ADC_CHANNEL_T)adc_get_selected_input(adc);
	/* Check if interrupt is enabled */
	bool irq = adc_get_base_reg(adc)->INTEN & (1UL << (uint8_t)channel);
	/* Start conversion if no interrupt is enabled */
	if(!irq) {
		/* Start conversion */
		adc_set_start_mode(adc, ADC_START_NOW);
		/* Wait for conversion to finish */
		while( !adc_get_conversion_state(adc, channel) );
	}
	/* Read value from channel */
	return adc_get_conversion_value(adc, channel);
}

/**
 * @brief Returns the conversion value
 * @param adc: ADC number
 * @param channel: ADC channel
 * @return ADC conversion value
 */
uint16_t adc_get_conversion_value(uint8_t adc, uint8_t channel) {
	/* Auxiliary variable */
	uint32_t temp;
	/* Get register value */
	temp = adc_get_base_reg(adc)->DR[channel];
	/* Return proper value */
	return (uint16_t) ADC_DR_RESULT(temp);
}

/**
 * @brief Enable/Disable interrupt
 * @param adc: ADC number
 * @param enabled: interrupt state
 */
void adc_set_irq_enabled(uint8_t adc, bool enabled) {
	/* ADC interrupts */
	const LPC43XX_IRQn_Type irqs[] = { ADC0_IRQn, ADC1_IRQn };
	/* Enable ADC interrupt */
	if(enabled) { NVIC_EnableIRQ(irqs[adc]); }
	/* Or disable ADC interrupt */
	else { NVIC_DisableIRQ(irqs[adc]); }
	/* Get the selected channel */
	uint8_t channel = adc_get_selected_input(adc);
	/* Enable ADC interrupt on channel */
	adc_set_irq_channel_enabled(adc, channel, enabled);
}

/**
 * @brief ADC0 interrupt handler
 */
void ADC0_IRQHandler(void) {
	/* Check if there is a handler and call it */
	if(adc_handlers[0]) { adc_handlers[0](); }
}

/**
 * @brief ADC1 interrupt handler
 */
void ADC1_IRQHandler(void) {
	/* Check if there is a handler and call it */
	if(adc_handlers[1]) { adc_handlers[1](); }
}
