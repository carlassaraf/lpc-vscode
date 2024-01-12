/*
 * ciaa_dac_api.c
 *
 *  Created on: Jun 18, 2022
 *      Author: fabri
 */

/*
 * ciaa_dac_api.c
 *
 *  Created on: Jun 4, 2022
 *      Author: fabri
 */


#include "ciaa_dac_api.h"

/*
 *	@brief	Initialize DAC with default settings
 *
 * 	@param	None
 *
 *	@return	None
 */
void dac_init(void) {
	/* Get default configuration */
	dac_config_t config = dac_get_default_config();
	/* Initialize DAC */
	dac_config_init(config);
}

/*
 *	@brief	Initialize DAC with given settings
 *
 * 	@param	config: DAC configuration struct
 *
 *	@return	None
 */
void dac_config_init(dac_config_t config) {
	/* Enable DAC clock */
	Chip_Clock_EnableOpts(CLK_APB3_DAC, true, true, 1);
	/* Set DAC bias */
	dac_set_bias(config.bias);
	/* Enable/Disable output */
	dac_set_enabled(config.enabled);
	/* Enable/Disable P4_4 output */
	dac_set_alternative_output_enabled(config.alternativeOutput);
	/* Set output value */
	dac_set_output_value(config.value);
}
