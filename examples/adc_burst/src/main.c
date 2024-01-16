/* Copyright 2024, Fabrizio Carlassara
 * All rights reserved.
 *
 * This file is part of LPC-VSCODE.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "ciaa_adc_api.h"

// ADC conversion flag
bool conversion_done = false;
// ADC result
uint16_t result;
// ADC conversion factor
const float conversion_factor = 3.3 / (1 << 10);

/**
 * @brief ADC interrupt handler
*/
void adc_conversion_done(void) {
	// Get ADC value (10 bits)
	result = adc_read(0);
	// Change flag value
	conversion_done = true;
}

/**
 * @brief Main program
*/
int main(void) {
	// Update system core clock
	SystemCoreClockUpdate();
	// Get ADC defau;t configuration
	adc_config_t config = adc_get_default_config();
	// Enable burst mode
	config.burstMode = ENABLE;
	// Enable interrupt
	config.interrupt = ENABLE;
	// Initialize ADC0 with said configuration
	adc_config_init(0, config);
	// Select channel 0
	adc_select_input(0, ADC_CH0);
	// Set interrupt handler
	adc_set_irq_handler(0, adc_conversion_done);
	// Enable interrupt on channel 0
	adc_set_irq_channel_enabled(0, ADC_CH0, true);

	while(1) {
		// Wait for conversion to be ready
		if(conversion_done) {
            // Get voltage value
            float voltage = result * conversion_factor;
			// Change flag status
			conversion_done = false;
		}
	}
}