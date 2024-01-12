/*
 * ciaa_dma_api.c
 *
 *  Created on: Nov 7, 2022
 *      Author: carlassaraf
 */

#include "ciaa_dma_api.h"

/* DMA channel handlers pointer */
void (*dma_handlers[])(void) = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

/**
 * @brief Initialize DMA controller
 */
void dma_init(void) {
	/* Initialize DMA controller */
	Chip_GPDMA_Init(LPC_GPDMA);
	/* Configure DMA interrupt */
	NVIC_DisableIRQ(DMA_IRQn);
	NVIC_SetPriority(DMA_IRQn, ((0x01 << 3) | 0x01));
	NVIC_EnableIRQ(DMA_IRQn);
}

/**
 * @brief Begins a DMA transfer
 * @param config: an instance of a DMA configuration struct
 */
void dma_transfer(dma_config_t config) {
	/* Setup DMA transfer */
	Chip_GPDMA_Transfer(
			LPC_GPDMA,
			config.channel,
			config.src,
			config.dst,
			(dma_connection_type_t) config.type,
			config.size
	);
}

/**
 * @brief DMA interrupt handler
 */
void DMA_IRQHandler(void) {
	/* Check every available DMA channel */
	for(uint8_t channel = 0; channel < GPDMA_NUMBER_CHANNELS; channel++) {
		/* Check DMA channel transfer status */
		if(Chip_GPDMA_Interrupt(LPC_GPDMA, channel) == SUCCESS) {
			/* Check if there's any handler */
			if(dma_handlers[channel]) {
				/* Call it */
				dma_handlers[channel]();
				/* Return from handler */
				return;
			}
		}
	}
}
