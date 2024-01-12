/*
 * ciaa_board_api.c
 *
 *  Created on: Sep 8, 2022
 *      Author: carlassaraf
 */

#include "ciaa_board_api.h"

/*
 *	@brief	Initializes common CIAA board GPIO
 *
 *	@param	None
 *
 *	@return	None
 */
void ciaa_board_init(void) {
	/* Update system clock */
    SystemCoreClockUpdate();
    /* Initialize LEDs as output */
    gpio_set_dir(LED1, true);
    gpio_set_dir(LED2, true);
    gpio_set_dir(LED3, true);
    gpio_set_dir(LEDR, true);
    gpio_set_dir(LEDB, true);
    gpio_set_dir(LEDG, true);
    /* Turn off LEDs */
    gpio_clr(LED1);
    gpio_clr(LED2);
    gpio_clr(LED3);
    gpio_clr(LEDR);
    gpio_clr(LEDB);
    gpio_clr(LEDG);
    /* Initialize push buttons */
    gpio_set_dir(TEC1, false);
    gpio_set_dir(TEC2, false);
    gpio_set_dir(TEC3, false);
    gpio_set_dir(TEC4, false);
}
