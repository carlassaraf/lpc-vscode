/*
 * ciaa_uart_api.h
 *
 *  Created on: Jun 9, 2022
 *      Author: fabri
 */

#ifndef CIAA_UART_API_H_
#define CIAA_UART_API_H_

#include "chip.h"
#include "ciaa_scu_api.h"
#include "ciaa_gpio_api.h"

/* USART base register definitions */
#define CIAA_UART0	LPC_USART0
#define CIAA_UART1	LPC_UART1
#define CIAA_UART2	LPC_USART2
#define CIAA_UART3	LPC_USART3

/* UART definitions */
typedef enum {
	UART_0,
	UART_1,
	UART_2,
	UART_3
} ciaa_uart_t;

/* UART line status definitions */
typedef enum {
	UART_RECEIVE_DATA_READY = (1 << 0),		/* Line status: Receive data ready */
	UART_OVERRUN_ERROR	= (1 << 1),			/* Line status: Overrun error */
	UART_PARITY_ERROR = (1 << 2),			/* Line status: Parity error */
	UART_FRAMING_ERROR = (1 << 3),			/* Line status: Framing error */
	UART_BREAK_INTERRUPT = (1 << 4),		/* Line status: Break interrupt */
	UART_HOLDING_REG_EMPTY = (1 << 5),		/* Line status: Transmit holding register empty */
	UART_TX_EMPTY = (1 << 6),				/* Line status: Transmitter empty */
	UART_RX_FIFO_ERROR = (1 << 7),			/* Line status: Error in RX FIFO */
	UART_TX_FIFO_ERROR = (1 << 8)			/* Line status: Error in RX FIFO */
} uart_line_status_t;

/* UART Stop bit select bit definitions */
typedef enum {
	UART_STOP_BITS_1 = (0 << 2),			/* UART stop bit select: 1 stop bit */
	UART_STOP_BITS_2 = (1 << 2)    			/* UART stop bit select: 2 stop bits (in 5 bit data mode, 1.5 stop bits) */
} uart_stop_bits_t;

/* UART Parity enable bit definitions */
typedef enum {
	UART_PARITY_ENABLE = (1 << 3),			/* UART Parity Enable */
	UART_PARITY_DISABLE = (0 << 3),			/* UART Parity Disable */
	UART_PARITY_ODD = (0 << 4),				/* UART Parity select: Odd parity */
	UART_PARITY_EVEN = (1 << 4),			/* UART Parity select: Even parity */
	UART_STICK_1 = (2 << 4),				/* UART Parity select: Forced 1 stick parity */
	UART_STICK_2 = (3 << 4)					/* UART Parity select: Forced 0 stick parity */
} uart_parity_t;

/* UART word length select bit definitions */
typedef enum {
	UART_WORD_LENGTH_5 = (0 << 0),			/* UART word length select: 5 bit data mode */
	UART_WORD_LENGTH_6 = (1 << 0),			/* UART word length select: 6 bit data mode */
	UART_WORD_LENGTH_7 = (2 << 0),			/* UART word length select: 7 bit data mode */
	UART_WORD_LENGTH_8 = (3 << 0)			/* UART word length select: 8 bit data mode */
} uart_word_length_t;

/* UART Interrupt enable */
typedef enum {
	UART_RECEIVE_AVAILABLE_IRQ = (1 << 0),	/* RBR Interrupt enable */
	UART_HOLDING_REG_EMPTY_IRQ = (1 << 1),	/* THR Interrupt enable */
	UART_LINE_STATUS_IRQ = (1 << 2)			/* RX line status interrupt enable */
} uart_irqs_t;

/* UART Interrupt IDs */
typedef enum {
	UART_RLS = (0x3 << 1),					/* Receive Line Status */
	UART_RDA = (0x2 << 1),					/* Receive Data Available */
	UART_CTI = (0x6 << 1),					/* Character Time-out Indicator */
	UART_THRE = (0x1 << 1),					/* Transmitter Holding Register Empty */
} uart_irq_ids_t;

/* UART config struct */
typedef struct {
	ciaa_uart_t uart;						/* UART type */
	uint32_t baudRate;						/* UART baud rate */
	uart_stop_bits_t stopBits;				/* UART stop bits */
	uart_parity_t parityBits;				/* UART parity bits */
	uart_word_length_t wordLength;			/* UART word length */
	pin_t *txPin;							/* UART TXD pin */
	pin_t *rxPin;							/* UART RXD pin */
} uart_config_t;

/* Extern UART base register array */
extern LPC_USART_T *CIAA_UARTS[];
/* Extern UART default configurations */
extern uart_config_t UART_DEFAULT_CONFIGS[];
/* Extern interrupt handler function pointer */
extern void (*uart_handlers[])(void);

/* Function prototypes */
void uart_init(ciaa_uart_t uart);
void uart_config_init(uart_config_t config);
void uart_puts(ciaa_uart_t uart, const char* str);
void uart_gets(ciaa_uart_t uart, char *str);
void uart_set_baud_rate(ciaa_uart_t usart, uint32_t baudRate);

/* Inline functions */

/*
 * 	@brief	Get UART base register
 *
 * 	@param	uart: UART number
 *
 * 	@return UART base register
 */
static inline LPC_USART_T* uart_get_base_register(ciaa_uart_t uart) { return CIAA_UARTS[uart]; }

/*
 * 	@brief	Get configuration value for word length, stop and parity bits
 *
 * 	@param	config: UART configuration
 *
 * 	@return	configuration value
 */
static inline uint8_t uart_get_config_value(uart_config_t config) { return (config.stopBits | config.parityBits | config.wordLength); }

/*
 * 	@brief	Set stop, parity and word length
 *
 * 	@param	config: UART configuration
 *
 * 	@return	None
 */
static inline void uart_set_config_value(uart_config_t config) { uart_get_base_register(config.uart)->LCR = uart_get_config_value(config); }

/*
 * 	@brief	Get default configuration for UART
 *
 * 	@param	uart: UART number
 *
 * 	@return	UART default configuration
 */
static inline uart_config_t uart_get_default_config(ciaa_uart_t uart) { return UART_DEFAULT_CONFIGS[uart]; }

/*
 * 	@brief	Setup UART FIFOs
 *
 * 	@param	uart: UART number
 *
 * 	@return	None
 */
static inline void uart_setup_fifos(ciaa_uart_t uart) { uart_get_base_register(uart)->FCR = (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS); }

/*
 * 	@brief	Set TXD pin
 *
 * 	@param	uart: UART number
 * 	@param	pin: pin to assign
 *
 * 	@return	None
 */
static inline void uart_set_tx_pin(ciaa_uart_t uart, pin_t *pin) { scu_set_pin_mode(pin->SCU_PORT, pin->SCU_PIN, (SCU_MODE_INACT | pin->FUNCTIONS->USART)); }

/*
 * 	@brief	Set RXD pin
 *
 * 	@param	uart: UART number
 * 	@param	pin: pin to assign
 *
 * 	@return None
 */
static inline void uart_set_rx_pin(ciaa_uart_t uart, pin_t *pin) { scu_set_pin_mode(pin->SCU_PORT, pin->SCU_PIN, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | pin->FUNCTIONS->USART)); }

/*
 * 	@brief	Enable transmitter
 *
 * 	@param	uart: UART number
 *
 * 	@return	None
 */
static inline void uart_tx_enable(ciaa_uart_t uart) { uart_get_base_register(uart)->TER2 = UART_TER2_TXEN; }

/*
 * 	@brief	Disable transmitter
 *
 * 	@param	uart: UART number
 *
 * 	@return None
 */
static inline void uart_tx_disable(ciaa_uart_t uart) { uart_get_base_register(uart)->TER2 = 0; }

/*
 * 	@brief	Enable/Disable transmitter
 *
 * 	@param	uart: UART number
 * 	@param	enabled: whether the transmitter is enabled or not
 */
static inline void uart_set_tx_enabled(ciaa_uart_t uart, bool enabled) {
	/* Disable transmitter */
	uart_tx_disable(uart);
	/* Enable if necessary */
	if(enabled) { uart_tx_enable(uart); }
}

/*
 * 	@brief	Get UART line status
 *
 * 	@param	uart: UART number
 *
 * 	@return None
 */
static inline uart_line_status_t uart_get_line_status(ciaa_uart_t uart) { return (uart_line_status_t) uart_get_base_register(uart)->LSR; };

/*
 * 	@brief	Check if UART is available for reading
 *
 * 	@param	uart: UART number
 *
 * 	@return	true if data is available, false if not
 */
static inline bool uart_is_readable(ciaa_uart_t uart) { return (bool) (uart_get_line_status(uart) & UART_RECEIVE_DATA_READY); }

/*
 * 	@brief	Check if UART is available for writing
 *
 * 	@param	uart: UART number
 *
 * 	@return	true if data is available, false if not
 */
static inline bool uart_is_writable(ciaa_uart_t uart) { return (bool) (uart_get_line_status(uart) & UART_HOLDING_REG_EMPTY); }

/*
 * 	@brief	Write a byte through UART
 *
 * 	@param	uart: UART number
 * 	@param	data: byte to send
 *
 * 	@return	None
 */
static inline void uart_putc(ciaa_uart_t uart, char data) { uart_get_base_register(uart)->THR = (uint32_t) data; }

/*
 * 	@brief	Read a byte from UART
 *
 * 	@param	uart: UART number
 *
 * 	@return	byte read
 */
static inline const char uart_getc(ciaa_uart_t uart) { return (const char) (uart_get_base_register(uart)->RBR & UART_RBR_MASKBIT); }

/*
 * 	@brief	Enable/Disable UART RXD interrupt
 *
 * 	@param	uart: UART number
 * 	@param	enabled: whether the interrupt is enabled or not
 *
 * 	@return	None
 */
static inline void uart_set_rx_irq_enabled(ciaa_uart_t uart, bool enabled) {
	/* Interrupt vectors */
#ifdef CORE_M0
	LPC43XX_M0_IRQn_Type irqs[] = { USART0_IRQn, UART1_IRQn, USART2_IRQn, USART3_IRQn };
#elif defined(CORE_M4)
	LPC43XX_IRQn_Type irqs[] = { USART0_IRQn, UART1_IRQn, USART2_IRQn, USART3_IRQn };
#endif
	/* Disable RXD interrupt */
	uart_get_base_register(uart)->IER &= ~UART_RECEIVE_AVAILABLE_IRQ;
	NVIC_DisableIRQ(irqs[uart]);
	/* Enable iterrupt if necessary */
	if(enabled) {
		uart_get_base_register(uart)->IER |= UART_RECEIVE_AVAILABLE_IRQ;
		NVIC_EnableIRQ(irqs[uart]);
	}
}

/*
 * 	@brief	Set an RXD interrupt handler
 *
 * 	@param	uart: UART number
 * 	@param	handler: pointer to the handler function
 *
 * 	@return	None
 */
static inline void uart_set_rx_handler(ciaa_uart_t uart, void (*handler)(void)) { uart_handlers[uart] = handler; }

/*
 * 	@brief	Check if there is an interrupt pending
 *
 * 	@param	uart: UART number
 *
 * 	@return pending interrupt if available
 */
static inline uart_irq_ids_t uart_get_interrupt_pending(ciaa_uart_t uart) { return (uart_irq_ids_t) (uart_get_base_register(uart)->IIR & UART_IIR_INTID_MASK); }

#endif /* CIAA_UART_API_H_ */
