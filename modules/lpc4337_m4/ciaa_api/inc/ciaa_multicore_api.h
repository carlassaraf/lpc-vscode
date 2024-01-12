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

#ifdef CORE_M0
#define CPUID_CURR	CM0
#elif defined(CORE_M4)
#define	CPUID_CURR	CM4
#endif

#ifdef CORE_M0
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
#elif defined(CORE_M4)
/*
 * 	@brief	Alternative macro for the M0APP_IRQHandler
 *
 * 	@param	handler: function to execute when interrupt is triggered
 */
#define m0app_handler(handler);		void M0APP_IRQHandler(void) {	\
										/* Clear interrupt */		\
										multicore_irq_clear();		\
										/* Call handler */			\
										handler();					\
									}
#endif

#if !defined(CORE_M0) && defined(CORE_M4)
/* Function prototypes */
int multicore_m0_start(void);
#endif

/* Multicore inline functions */
#if !defined(CORE_M0) && defined(CORE_M4)
/*
 * 	@brief	Check memory alignment
 *
 * 	@param	address: address to the M0APP flash memory
 *
 * 	@return memory alignment value
 */
static inline uint32_t multicore_m0_check_alignment(uint32_t address) { return address & 0xFFF; }

/*
 * 	@brief	Perform a sanity check on the M0 memory address
 *
 * 	@param	address: address to the M0APP flash memory
 *
 * 	@return	true if valid, false if not
 */
static inline bool multicore_m0_sanity_check(uint32_t address) { return (address & 0xFFF00000) != ((*(unsigned long*) (address + 4)) & 0xFFF00000); }

/*
 * 	@brief	Set M0 memory map
 *
 * 	@param	address: address to the M0APP flash memory
 *
 * 	@return	None
 */
static inline void multicore_m0_set_memory_map(uint32_t address) { LPC_CREG->M0APPMEMMAP = address & ~0xFFF; }
#endif

#ifdef CORE_M0
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

#elif defined(CORE_M4)
/*
 * 	@brief	Enable/Disbale M0app interrupt for M4 core
 *
 * 	@param	enabled: whether to enable or disable
 *
 * 	@return	None
 */
static inline void multicore_m0app_irq_set_enabled(bool enabled) {
	if(enabled) { NVIC_EnableIRQ(M0APP_IRQn); }
	else { NVIC_DisableIRQ(M0APP_IRQn); }
}
#endif

/*
 * 	@brief	Initiate interrupt on other processor
 *
 * 	@param	None
 *
 * 	@return	None
 */
#ifdef CORE_M0
static inline void multicore_interrupt_m4_core(void) {
#elif defined(CORE_M4)
static inline void multicore_interrupt_m0_core(void) {
#endif
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
#ifdef CORE_M0
	LPC_CREG->M4TXEVENT = 0;
#elif defined(CORE_M4)
	LPC_CREG->M0APPTXEVENT = 0;
#endif
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
#ifdef CORE_M0
	multicore_irq_handler[CM4] = handler;
#elif defined(CORE_M4)
	multicore_irq_handler[CM0] = handler;
#endif
}

#endif /* CIAA_MULTICORE_API_H_ */
