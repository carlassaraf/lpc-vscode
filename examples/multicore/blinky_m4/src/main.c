/* Copyright 2016, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of Workspace.
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

#include "ciaa_gpio_api.h"
#include "ciaa_systick_api.h"
#include "ciaa_stdlib.h"
#include "ciaa_multicore_api.h"

// Counts the number of Systick interrupts
uint8_t systick_cnt = 0;
// Counts the number of M0 Core interrupts
uint8_t m0_cnt = 0;

/**
 * @brief M0 core interrupt callback
*/
void m0app_callback(void) {
	// Toggle LED every 2 M0 to M4 interrupts
	if(m0_cnt++ == 1) {
		// Toggle LED3
		gpio_xor(LED3);
		// Reset counter
		m0_cnt = 0;
	}
}

/**
 * @brief Systick interrupt callback
*/
void systick_callback(void) {
	// Interrupt M0 at 500 ms
	if(systick_cnt++ == 9) {
		// Interrupt M0 core
		multicore_interrupt_m0_core();
		// Reset counter
		systick_cnt = 0;
	}
}

/**
 * @brief Main program
*/
int main(void) {
	// Update system clock
	SystemCoreClockUpdate();
	// Set LED1 and LED3 as output
	gpio_set_dir_out(LED1);
	gpio_set_dir_out(LED3);
	// Enable interrupt from M0 core
	multicore_m0app_irq_set_enabled(true);
	// Start M0 core
	multicore_m0_start();
	// Clear LED1 and LED3
	gpio_clr(LED1);
	gpio_clr(LED3);
	// Initialize Systick with 1 ms
	systick_init(50000);
	// Add Systick callback
	systick_set_irq_handler(systick_callback);

	while (1) {
		// Wait for 1 second
		sleep_ms(2000);
		// Toggle LED1
		gpio_xor(LED1);
	}
	return 0;
}

/**
 * @brief M0APP interrupt handler
 * @note Produces the same effect as using macro: m0app_handler(m0app_callback);
*/
void M0APP_IRQHandler(void) {
	// Clear interrupt
	multicore_irq_clear();
	// Call callback
	m0app_callback();
}