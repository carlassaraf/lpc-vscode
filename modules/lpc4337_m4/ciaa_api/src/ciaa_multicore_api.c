/*
 * ciaa_multicore_api.c
 *
 *  Created on: Jun 16, 2022
 *      Author: fabri
 */

#include "ciaa_multicore_api.h"

/* Pointer array to interrupt handlers */
void (*multicore_irq_handler[])(void) = { NULL, NULL };

#if !defined(CORE_M0) && defined(CORE_M4)
/*
 * 	@brief	Start M0 core
 *
 * 	@param	None
 *
 * 	@return	zero if initialized correctly
 */
int multicore_m0_start(void) {
	/* Base flash address for M0 core */
	uint32_t m0_image_addr = BASE_ADDRESS_M0APP;
	/* Make sure the alignment is OK */
	if (multicore_m0_check_alignment(m0_image_addr)) { return -1; }
	/* Sanity check, see if base address and reset handler address resides in same region */
	if (multicore_m0_sanity_check(m0_image_addr)) { return -2; }
	/* Make sure the M0 core is being held in reset via the RGU */
	Chip_RGU_TriggerReset(RGU_M0APP_RST);
	/* Enable M0 clock at 204MHz */
	Chip_Clock_Enable(CLK_M4_M0APP);
	/* Keep in mind the M0 image must be aligned on a 4K boundary */
	multicore_m0_set_memory_map(m0_image_addr);
	/* Clear M0 core reset */
	Chip_RGU_ClearReset(RGU_M0APP_RST);
	/* Return 0 if initialization was successful */
	return 0;
}
#endif
