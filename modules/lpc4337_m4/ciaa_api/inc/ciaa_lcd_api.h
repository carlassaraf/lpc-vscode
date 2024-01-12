/*
 * ciaa_lcd_api.h
 *
 *  Created on: Jun 12, 2022
 *      Author: fabri
 */

#ifndef CIAA_LCD_API_H_
#define CIAA_LCD_API_H_

#include "chip.h"
#include "ciaa_gpio_api.h"
#include "ciaa_stdlib.h"

/* LCD struct with pointers to pins */
typedef struct {
	pin_t *en;		/* Enable pin pointer */
	pin_t *rs;		/* Register Select pin pointer */
	pin_t *d4;		/* Data pin 4 pointer */
	pin_t *d5;		/* Data pin 5 pointer */
	pin_t *d6;		/* Data pin 6 pointer */
	pin_t *d7;		/* Data pin 7 pointer */
} lcd_t;

/* LCD configuration struct */
typedef struct {
	lcd_t lcd;		/* Pin struct */
	bool cursor;	/* Cursor enabled or not */
	bool blink;		/* Blink cursor or not */
	bool bigFont;	/* 5x10 or 5x7 font */
} lcd_config_t;

/* Function prototypes */
void lcd_init(void);
void lcd_config_init(lcd_config_t lcd);
void lcd_put_nibble(lcd_t lcd, uint8_t nibble);
void lcd_put_command(lcd_t lcd, uint8_t cmd);
void lcd_putc(lcd_t lcd, char c);
void lcd_puts(lcd_t lcd, const char* str);
void lcd_clear(lcd_t lcd);
void lcd_go_to_xy(lcd_t lcd, uint8_t x,  uint8_t y);

/* Inline functions */

/*
 * 	@brief	Get LCD default configuration
 *
 * 	@prarm	None
 *
 * 	@return	LCD default configuration struct
 */
static inline lcd_config_t lcd_get_default_config(void) {
	lcd_config_t config = {
		{ &LCD_EN, &LCD_RS, &LCD1, &LCD2, &LCD3, &LCD4 },	/* LCD default pins */
		false,												/* No cursor */
		false,												/* No blink */
		false												/* 5x7 font */
	};
	return config;
}

/*
 * 	@brief	Get LCD pins from config struct
 *
 * 	@param	config: LCD configuration struct
 *
 * 	@param	return: LCD pin struct
 */
static inline lcd_t lcd_get_from_config(lcd_config_t config) {
	return config.lcd;
}

#endif /* CIAA_LCD_API_H_ */
