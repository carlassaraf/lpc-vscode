/*
 * ciaa_pwm_api.h
 *
 *  Created on: Jun 5, 2022
 *      Author: fabri
 */

#ifndef CIAA_PWM_API_H_
#define CIAA_PWM_API_H_

#include "chip.h"
#include "ciaa_gpio_api.h"

/* SCT base register redefinition */
#define	CIAA_PWM	LPC_SCT

/* PWM output struct */
typedef struct {
	pin_t *pin;			/* Pointer to a Pin struct */
	uint8_t index;		/* CTOUT index (0 to 15) */
	uint8_t pwm;		/* PWM index (1 to 16) */
} pwm_output_t;

/* PWM configuration struct */
typedef struct {
	uint32_t frequency;	/* PWM frequency */
	uint8_t clkdiv;		/* PWM clock divider */
} pwm_config_t;

/* CTOUT available indexes */
typedef enum {
	CTOUT_0, CTOUT_1, CTOUT_2, CTOUT_3,
	CTOUT_4, CTOUT_5, CTOUT_6, CTOUT_7,
	CTOUT_8, CTOUT_9, CTOUT_10, CTOUT_11,
	CTOUT_12, CTOUT_13, CTOUT_14, CTOUT_15
} ctout_t;

/* PWM output declarations */
extern pwm_output_t PWM0;
extern pwm_output_t PWM1;
extern pwm_output_t PWM2;
extern pwm_output_t PWM3;
extern pwm_output_t PWM4;
extern pwm_output_t PWM5;
extern pwm_output_t PWM6;
extern pwm_output_t PWM7;
extern pwm_output_t PWM8;
extern pwm_output_t PWM9;
extern pwm_output_t PWM10;
extern pwm_output_t PWM11;
extern pwm_output_t PWM12;
extern pwm_output_t PWM13;
extern pwm_output_t PWM14;

/* Function prototypes */
void pwm_init(void);
void pwm_config_init(pwm_config_t config);
void pwm_set_frequency(uint32_t frequency);
void pwm_set_output_pin(pwm_output_t output);

/* Inline functions */

/*
 * 	@brief	Get default PWM configuration
 *
 * 	@param	None
 *
 * 	@return	PWM configuration struct
 */
static inline pwm_config_t pwm_get_default_config(void) {
	/* Declare default configuration: 1KHz frequency, Clock Div 1 */
	pwm_config_t config = { 1000, 1 };
	return config;
}

/*
 * 	@brief	Start SCT counter
 *
 * 	@param	None
 *
 * 	@return	None
 */
static inline void pwm_start(void) { CIAA_PWM->CTRL_U &= ~(SCT_CTRL_HALT_L | SCT_CTRL_HALT_H); }

/*
 * 	@brief	Stop SCT counter
 *
 * 	@param	None
 *
 * 	@return None
 */
static inline void pwm_stop(void) {
	/* Stop the timer */
	CIAA_PWM->CTRL_U |= (SCT_CTRL_HALT_L | SCT_CTRL_HALT_H);
	/* Clear counter */
	CIAA_PWM->CTRL_U |= (SCT_CTRL_CLRCTR_L | SCT_CTRL_CLRCTR_H);
}

/*
 * 	@brief	Enable/Disable SCT counter
 *
 * 	@param	enabled: whether the counter is enabled or not
 *
 * 	@return	None
 */
static inline void pwm_set_start(bool enabled) {
	/* If enabled, start the counter */
	if(enabled) { pwm_start(); }
	/* Stop ip if not */
	else { pwm_stop(); }
}

/*
 *	@brief	Set new match value
 *
 *	@param	match: match output to update
 *	@param	value: number of ticks for the match
 *
 *	@return	None
 */
static inline void pwm_set_match(uint8_t match, uint32_t value) { CIAA_PWM->MATCHREL[match].U = value; }

/*
 * 	@brief	Set counter max ticks
 *
 * 	@param	ticks: number of ticks to set
 *
 * 	@return	None
 */
static inline void pwm_set_wrap(uint32_t value) {
	/* Stop the SCT before configuration */
	pwm_stop();
	/* Reset CTOUT 0 counter value */
	pwm_set_match(SCT_MATCH_0, 0);
	/* Update wrap value */
	CIAA_PWM->MATCHREL[SCT_MATCH_0].U = value;
	/* Start SCT counter */
	pwm_start();
}

/*
 * 	@brief	Get counter max ticks
 *
 * 	@param	None
 *
 * 	@param	max number of ticks
 */
static inline uint32_t pwm_get_wrap(void) { return CIAA_PWM->MATCHREL[SCT_MATCH_0].U; }

/*
 * 	@brief	Convert duty percent to ticks
 *
 * 	@param	percent: 0-100 value
 *
 * 	@return	ticks value
 */
static inline uint32_t pwm_percent_to_ticks(uint8_t percent) {
	/* Check the value is within limits */
	if(percent > 0 && percent <= 100) {	return (pwm_get_wrap() * percent) / 100; }
	return 0;
}


/*
 * 	@brief	Convert duty ticks to percent
 *
 * 	@param	ticks: number of ticks to convert
 *
 * 	@return	0-100 percent value
 */
static inline uint8_t pwm_ticks_to_percent(uint32_t ticks) { return (100 * ticks) / pwm_get_wrap(); }

/*
 *	@brief	Set duty cycle in ticks
 *
 *	@param	output: PWM output to change
 *	@param	ticks: duty cycle in number of ticks
 *
 *	@return	None
 */
static inline void pwm_set_duty_ticks(pwm_output_t output, uint32_t ticks) { pwm_set_match(output.pwm, ticks); }

/*	@brief	Get duty cycle in ticks
 *
 * 	@param	output: PWM output to change
 *
 * 	@return	number of match ticks
 */
static inline uint32_t pwm_get_duty(pwm_output_t output) { return CIAA_PWM->MATCHREL[output.pwm].U; }

/*
 *	@brief	Set duty cycle in percent
 *
 *	@param	output: PWM output to change
 *	@param	percent: duty cycle in percent. 0-100 value
 *
 *	@return None
 */
static inline void pwm_set_duty_percent(pwm_output_t output, uint8_t percent) { pwm_set_duty_ticks(output, pwm_percent_to_ticks(percent)); }

/*
 * 	@brief	Set duty cycle in us
 *
 * 	@param	output: PWM output to change
 * 	@param	us: duty cycle in microseconds
 *
 * 	@return	None
 */
static inline void pwm_set_duty_us(pwm_output_t output, uint32_t us) {
	/* Get PWM frequency */
	uint32_t freq = Chip_Clock_GetRate(CLK_MX_SCT) / pwm_get_wrap();
	/* Get number of ticks */
	uint32_t ticks = pwm_get_wrap() * ((float)us / 1E6) * freq;
	/* Set duty cycle in ticks */
	pwm_set_duty_ticks(output, ticks);
}


#endif /* CIAA_PWM_API_H_ */
