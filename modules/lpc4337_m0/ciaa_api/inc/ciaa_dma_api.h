/*
 * ciaa_dma_api.h
 *
 *  Created on: Nov 7, 2022
 *      Author: carlassaraf
 */

#ifndef CIAA_DMA_API_H_
#define CIAA_DMA_API_H_

#include "chip.h"

/**
 * @brief DMA possible connections
 */
typedef enum {
	conn_mem 			= 0UL,		/* MEMORY */
	conn_mat0_0 		= 1UL,		/* MAT0.0 */
	conn_uart0_tx 		= 2UL,		/* UART0 Tx */
	conn_mat0_1 		= 3UL,		/* MAT0.1 */
	conn_uart0_rx 		= 4UL,		/* UART0 Rx */
	conn_mat1_0			= 5UL,		/* MAT1.0 */
	conn_uart1_tx		= 6UL,		/* UART1 Tx */
	conn_mat1_1			= 7UL,		/* MAT1.1 */
	conn_uart1_rx		= 8UL,		/* UART1 Rx */
	conn_mat2_0			= 9UL,		/* MAT2.0 */
	conn_uart2_tx		= 10UL,		/* UART2 Tx */
	conn_mat2_1			= 11UL,		/* MAT2.1 */
	conn_uart2_rx		= 12UL,		/* UART2 Rx */
	conn_mat3_0			= 13UL,		/* MAT3.0 */
	conn_uart3_tx		= 14UL,		/* UART3 Tx */
	conn_sct_0			= 15UL,		/* SCT0 */
	conn_mat3_1			= 16UL,		/* MAT3.1 */
	conn_uart_rx		= 17UL,		/* UART3 Rx */
	conn_sct_1			= 18UL,		/* SCT1 */
	conn_ssp0_rx		= 19UL,		/* SSP0 Rx */
	conn_i2s0_tx_ch0	= 20UL,		/* I2S0 Tx Channel 0 */
	conn_ssp0_tx		= 21UL,		/* SSP0 Tx */
	conn_i2s0_rx_ch0	= 22UL,		/* I2S0 Rx Channel 0 */
	conn_ssp1_rx		= 23UL,		/* SSP1 Rx */
	conn_ssp1_tx		= 24UL,		/* SSP1 Tx */
	conn_adc0			= 25UL,  	/* ADC0 */
	conn_adc1			= 26UL,		/* ADC1 */
	conn_dac			= 27UL,		/* DAC */
	conn_i2s1_tx_ch0	= 28UL,		/* I2S1 Tx Channel 0 */
	conn_i2s1_rx_ch0	= 30UL,		/* I2S1 Rx Channel 0 */
} dma_conn_t;

/* DMA type redefinitions from SDK */
typedef DMA_ChannelHandle_t dma_channel_handler_t;
typedef GPDMA_FLOW_CONTROL_T dma_connection_type_t;

/**
 * @brief DMA configuration struct
 *
 * @note src and dst can be pointers to memory or any of
 * the above dma_conn_t values
 */
typedef struct {
	uint32_t channel;	/* DMA channel */
	uint32_t src;		/* DMA source */
	uint32_t dst;		/* DMA destiny */
	uint32_t size;		/* DMA buffer size */
	uint32_t type;		/* DMA connection type (dma_connection_type_t) */
} dma_config_t;

/* DMA channel handlers pointer */
extern void (*dma_handlers[])(void);

/* Function prototypes */

void dma_init();
void dma_transfer(dma_config_t config);

/**
 * @brief Get the first available DMA channel
 * @return DMA channel index
 */
static inline uint8_t dma_get_free_channel(void) { return Chip_GPDMA_GetFreeChannel(LPC_GPDMA, 0x00); }

/**
 * @brief Set a handler for a DMA channel interrupt
 * @param channel: DMA channel index
 * @param handler: pointer to a function
 */
static inline void dma_set_channel_handler(uint32_t channel, void (*handler)(void)) { dma_handlers[channel] = handler; }

#endif /* CIAA_DMA_API_H_ */
