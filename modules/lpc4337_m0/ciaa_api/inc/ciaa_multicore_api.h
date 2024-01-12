/*
 * ciaa_multicore_api_m4.h
 *
 *  Created on: Jun 16, 2022
 *      Author: fabri
 */

#ifndef CIAA_MULTICORE_API_H_
#define CIAA_MULTICORE_API_H_

#include "chip.h"

/* Size of applications in flash */
#define IMG_SZ_M4      (32 * 1024)
#define IMG_SZ_M0APP   (16 * 1024)
/* IFLASH BANK-A */
#define IFLASH_A_BASE_ADDR  	0x1A000000
/* IFLASH BANK-B */
#define IFLASH_B_BASE_ADDR 		0x1B000000
/* M0APP Flash address */
#define BASE_ADDRESS_M0APP     	(IFLASH_B_BASE_ADDR)
/* Shared IPC memory */
#define	SHARED_MEM_IPC			0x10088000

/* Extern interrupt handler */
extern void (*multicore_irq_handler[])(void);

/* Available coees */
typedef enum {
	CM4,
	CM0
} multicore_cores_t;

/*
 * 	@brief	Alternative macro for the M4_IRQHandler
 *
 * 	@param	handler: function to execute when interrupt is triggered
 */
#define m4_handler(handler); 	void M4_IRQHandler(void) {	\
									/* Clear interrupt */	\
									multicore_irq_clear();	\
									/* Call handler */		\
									handler();				\
								}

/*
 * 	@brief	Enable/Disbale M4 interrupt for M0 core
 *
 * 	@param	enabled: whether to enable or disable
 *
 * 	@return	None
 */
static inline void multicore_m4_irq_set_enabled(bool enabled) {
	if(enabled) { NVIC_EnableIRQ(M4_IRQn); }
	else { NVIC_DisableIRQ(M4_IRQn); }
}

/*
 * 	@brief	Initiate interrupt on other processor
 *
 * 	@param	None
 *
 * 	@return	None
 */
static inline void multicore_interrupt_m4_core(void) {
	__DSB();
	__SEV();
}

/*
 * 	@brief	Clear M4/M0 core interrupt
 *
 * 	@param	None
 *
 * 	@return	None
 */
static inline void multicore_irq_clear(void) {
	LPC_CREG->M4TXEVENT = 0;
}

/*
 * 	@brief	Set a handler for the M4/M0 interrupt
 *
 * 	@param	handler: function to call from interrupt
 *
 * 	@return	None
 *
 * 	@note	Not working for some reason
 */
static inline void multicore_irq_set_handler(void (*handler)(void)) {
	multicore_irq_handler[CM4] = handler;
}

#endif /* CIAA_MULTICORE_API_H_ */
