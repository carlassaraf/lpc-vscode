/*
 * ciaa_uart_api.c
 *
 *  Created on: Jun 9, 2022
 *      Author: fabri
 */

#include <ciaa_uart_api.h>

/* Available UARTs */
LPC_USART_T *CIAA_UARTS[] = { CIAA_UART0, CIAA_UART1, CIAA_UART2, CIAA_UART3 };
/* UART clocks */
static const CHIP_CCU_CLK_T CIAA_UART_CLKS[] = { CLK_MX_UART0, CLK_MX_UART1, CLK_MX_UART2, CLK_MX_UART3 };
/* UART default configurations */
uart_config_t UART_DEFAULT_CONFIGS[] = {
	{ UART_0, 115200, UART_STOP_BITS_1, UART_PARITY_DISABLE, UART_WORD_LENGTH_8, &GPIO1, &GPIO2 },
	{ UART_1, 115200, UART_STOP_BITS_1, UART_PARITY_DISABLE, UART_WORD_LENGTH_8, NULL, NULL },
	{ UART_2, 115200, UART_STOP_BITS_1, UART_PARITY_DISABLE, UART_WORD_LENGTH_8, &RXD0, &CRS_DV },
	{ UART_3, 115200, UART_STOP_BITS_1, UART_PARITY_DISABLE, UART_WORD_LENGTH_8, &RS485_TXD, &RS485_RXD }
};

/* Interrupt handler function pointer */
void (*uart_handlers[])(void) = { NULL, NULL, NULL, NULL };

/*
 * 	@brief	Initialize UART with default configuration
 *
 * 	@param	uart: UART number
 *
 * 	@return None
 */
void uart_init(ciaa_uart_t uart) {
	/* Get default UART configuration and initialize */
	uart_config_init(uart_get_default_config(uart));
}

/*
 * 	@brief	Initialize UART with given configuration
 *
 * 	@param	config: UART configuration
 *
 * 	@return	None
 */
void uart_config_init(uart_config_t config) {
	/* Get base register */
	LPC_USART_T *reg = uart_get_base_register(config.uart);
	/* Select TX pin */
	uart_set_tx_pin(config.uart, config.txPin);
	/* Select RX pin */
	uart_set_rx_pin(config.uart, config.rxPin);
	/* Enable UART clocking. UART base clock(s) must already be enabled */
	Chip_Clock_EnableOpts(CIAA_UART_CLKS[config.uart], true, true, 1);
	/* Enable FIFOs by default, reset them */
	uart_setup_fifos(config.uart);
    /* Disable Tx pin */
	uart_set_tx_enabled(config.uart, false);
    /* Disable interrupts */
	reg->IER = 0;
	/* Set LCR to default state */
	reg->LCR = 0;
	/* Set ACR to default state */
	reg->ACR = 0;
    /* Set RS485 control to default state */
	reg->RS485CTRL = 0;
	/* Set RS485 delay timer to default state */
	reg->RS485DLY = 0;
	/* Set RS485 addr match to default state */
	reg->RS485ADRMATCH = 0;
    /* Clear MCR */
    if (reg == CIAA_UART1) {
		/* Set Modem Control to default state */
		reg->MCR = 0;
		/* Configure USART */
	    volatile uint32_t tmp;
		/*Dummy Reading to Clear Status */
		tmp = reg->MSR;
	}
	/* Default 8N1, with DLAB disabled */
    uart_set_config_value(config);
	/* Disable fractional divider */
	reg->FDR = 0x10;
	/* Set USART baud rate */
	uart_set_baud_rate(config.uart, config.baudRate);
	/* Enable TX pin */
	uart_set_tx_enabled(config.uart, true);
}

/*
 * 	@brief	Set UART baud rate
 *
 * 	@param	uart: UART number
 * 	@param	baudRate: UART baud rate
 *
 * 	@return	None
 */
void uart_set_baud_rate(ciaa_uart_t usart, uint32_t baudRate) {
	/* Get USART base register */
	LPC_USART_T *reg = uart_get_base_register(usart);
	/* Clock divider variables */
	uint32_t div, divh, divl, clkin;
	/* Determine UART clock in rate without FDR */
	clkin = Chip_Clock_GetRate(CIAA_UART_CLKS[usart]);
	div = clkin / (baudRate * 16);
	/* High and low halves of the divider */
	divh = div / 256;
	divl = div - (divh * 256);
	/* Enable divisor access */
	reg->LCR |= UART_LCR_DLAB_EN;
	/* Set divisor latches */
	reg->DLL = divl;
	reg->DLM = divh;
	/* Disable divisor access */
	reg->LCR &= ~UART_LCR_DLAB_EN;
}

/*
 * 	@brief	Write a string through UART
 *
 * 	@param	uart: UART number
 * 	@param	str: string to send
 *
 * 	@return	None
 */
void uart_puts(ciaa_uart_t uart, const char* str) {
	/* Loop until null character */
	while(*str) {
		/* Wait until the transmit holding register is empty */
		while ( !uart_is_writable(uart) );
		/* Send byte */
		uart_putc(uart, *str++);
	}
}

/*
 * 	@brief	Read a string from UART
 *
 * 	@param	uart: UART number
 * 	@param	str: variable to store the string
 *
 * 	@return	None
 */
void uart_gets(ciaa_uart_t uart, char *str) {
	/* String size variable */
	uint8_t size = 0;
	/* Loop until null character */
	while(*str) {
		/* Clear character */
		*str = '\0';
		/* Increment size */
		size++;
		/* Increment pointer */
		str++;
	}
	/* Go back to first byte */
	str = (str - size);
	/* Wait until there is available data */
	while(!uart_is_readable(uart));
	/* Loop until no more data is available */
	while(uart_is_readable(uart)) {
		/* Read byte and save to pointer address */
		*str = uart_getc(uart);
		/* Increment pointer */
		str++;
	}
}

/*
 * 	@brief	UART0 interrupt handler
 *
 * 	@param	None
 *
 * 	@param	None
 */
void UART0_IRQHandler(void) {
	/* Check if there is available data to read */
	if(uart_get_interrupt_pending(UART_0) == UART_RDA) {
		/* Call handler if available */
		if(uart_handlers[UART_0]) { uart_handlers[UART_0](); }
	}
}

/*
 * 	@brief	UART1 interrupt handler
 *
 * 	@param	None
 *
 * 	@param	None
 */
void UART1_IRQHandler(void) {
	/* Check if there is available data to read */
	if(uart_get_interrupt_pending(UART_1) == UART_RDA) {
		/* Call handler if available */
		if(uart_handlers[UART_1]) { uart_handlers[UART_1](); }
	}
}

/*
 * 	@brief	UART2 interrupt handler
 *
 * 	@param	None
 *
 * 	@param	None
 */
void UART2_IRQHandler(void) {
	/* Check if there is available data to read */
	if(uart_get_interrupt_pending(UART_2) == UART_RDA) {
		/* Call handler if available */
		if(uart_handlers[UART_2]) { uart_handlers[UART_2](); }
	}
}

/*
 * 	@brief	UART3 interrupt handler
 *
 * 	@param	None
 *
 * 	@param	None
 */
void UART3_IRQHandler(void) {
	/* Check if there is available data to read */
	if(uart_get_interrupt_pending(UART_3) == UART_RDA) {
		/* Call handler if available */
		if(uart_handlers[UART_3]) { uart_handlers[UART_3](); }
	}
}
