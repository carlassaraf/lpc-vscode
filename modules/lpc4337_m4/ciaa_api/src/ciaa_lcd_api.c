/*
 * ciaa_lcd_api.c
 *
 *  Created on: Jun 13, 2022
 *      Author: fabri
 */

#include "ciaa_lcd_api.h"

/*
 * 	@brief	Initialize LCD with default configuration
 *
 * 	@param	None
 *
 * 	@return	None
 */
void lcd_init(void) {
	/* Get LCD default config and initialize */
	lcd_config_init(lcd_get_default_config());
}

/*
 * 	@brief	Initialize LCD with given configuration
 *
 * 	@param	config: LCD configuration
 *
 * 	@return	None
 */
void lcd_config_init(lcd_config_t config) {
	/* Get LCD struct */
	lcd_t lcd = lcd_get_from_config(config);
	/* Get pointer to the pointer of the first pin in struct */
	pin_t **p = &(lcd.en);
	/* Get the number of elements to loop through */
	uint8_t len = sizeof(lcd) / sizeof(pin_t*);
	/* Loop from first to last element */
	for(uint8_t i = 0; i < len; i++) {
		/* Get pointer to the pin */
		pin_t *pin = *p;
		/* Initialize pin */
		gpio_set_dir_out(*pin);
		/* Increment pointer to the next pin */
		p++;
	}

	/* Function set: 8 bits interface length */
	lcd_put_command(lcd, 0x03);
	/* Wait for 4.1 ms */
	sleep_ms(5);
	/* Function set: 8 bits interface length */
	lcd_put_command(lcd, 0x03);
	/* Wait for 100 us */
	sleep_us(100);
	/* Function set: 8 bits interface length */
	lcd_put_command(lcd, 0x03);
	/* Now the other instructions can be send */

	/* Function set: set interface to 4 bits length */
	lcd_put_command(lcd, 0x02);
	/* Function set: interface is 4 bits length */
	lcd_put_command(lcd, 0x02);
	/* Function set: two row display and 5x7 font */
  	lcd_put_command(lcd, 0x08 | (config.bigFont << 2));
  	/* Display on, cursor off, blink off */
  	lcd_put_command(lcd, 0x00);
  	lcd_put_command(lcd, (3 << 2) | (config.cursor << 1) | config.blink);
  	/* Input set command: increment cursor */
  	lcd_put_command(lcd, 0x00);
  	lcd_put_command(lcd, 0x06);
}

/*
 * 	@brief	Write a new nibble in data pins
 *
 * 	@param	lcd: LCD pin struct
 * 	@param	nibble: four bit data
 *
 * 	@return	None
 */
void lcd_put(lcd_t lcd, uint8_t nibble) {
	/* Get pointer to enable pin */
	pin_t *en = lcd.en;
	/* Get pointer to the pointer of the first pin in struct */
	pin_t **p = &(lcd.d4);
	/* Get the number of elements to loop through */
	uint8_t len = sizeof(lcd) / sizeof(pin_t*);
	/* Loop from third to last element */
	for(uint8_t i = 0; i < len - 2; i++) {
		/* Get pointer to the pin */
		pin_t *pin = *p;
		/* Set value to pin */
		gpio_put(*pin, (nibble & (1 << i))? true : false);
		/* Increment pointer to the next pin */
		p++;
	}
	/* Set enable pin */
	gpio_set(*en);
	/* Wait for 40 us */
	sleep_us(40);
	/* Clear enable pin */
	gpio_clr(*en);
}

/*
 * 	@brief	Write a new command in data pins
 *
 * 	@param	lcd: LCD pin struct
 * 	@param	cmd: four bit command
 *
 * 	@return	None
 */
void lcd_put_command(lcd_t lcd, uint8_t cmd) {
	/* Get pointer to rs pin */
	pin_t *rs = lcd.rs;
	/* Clear rs pin */
	gpio_clr(*rs);
	/* Put command in data pins */
	lcd_put(lcd, cmd);
}

/*
 * 	@brief	Write a new character in data pins
 *
 * 	@param	lcd: LCD pin struct
 * 	@param	c: char to be sent
 */
void lcd_putc(lcd_t lcd, char c) {
	/* Get pointer to rs pin */
	pin_t *rs = lcd.rs;
	/* Set rs pin */
	gpio_set(*rs);
	/* Repeat twice*/
	for(uint8_t nibble = 0; nibble < 2; nibble++) {
		/* Send lower nibble if it's the first time. Send higher nibble if second */
		uint8_t n = (nibble)? c & 0x0f : c >> 4;
		/* Write nibble */
		lcd_put(lcd, n);
	}
}

/*
 * 	@brief	Write a new string in LCD
 *
 * 	@param	lcd: LCD pin struct
 * 	@param	str: string to be sent
 *
 * 	@return None
 */
void lcd_puts(lcd_t lcd, const char* str) {
	/* Loop intul null character */
	while(*str) {
		/* Write the char */
		lcd_putc(lcd, *str);
		/* Increment pointer */
		str++;
	}
}

/*
 * 	@brief	Clear screen screen
 *
 * 	@oaram	lcd: LCD pin struct
 *
 * 	@return None
 */
void lcd_clear(lcd_t lcd) {
	/* Send clear command:  first nibble */
	lcd_put_command(lcd, 0x00);
	/* Send clear command: second nibble */
	lcd_put_command(lcd, 0x01);
	/* Wait for 4ms */
	sleep_ms(4);
}

/*
 * 	@brief	Go to string coordinate
 *
 * 	@param	lcd: LCD pin struct
 * 	@param	x: column number (starting in 0)
 * 	@param	y: row number (string in 0)
 */
void lcd_go_to_xy(lcd_t lcd, uint8_t x, uint8_t y) {
	/* Auxiliary variable */
	uint8_t aux;
	/* Check if the first row (0) is required */
	if(y == 0) {
		/* Set direction */
		aux = 0x40 + x;
		/* Send first nibble */
		lcd_put_command(lcd, aux >> 4);
		/* Send seconds byte */
		lcd_put_command(lcd, aux & 0x0F);
	}
	/* Check if the second row (2) is required  */
	else if (y == 1) {
		/* Set direction */
		aux = 0xC0 + x;;
		/* Send first nibble */
		lcd_put_command(lcd, aux >> 4);
		/* Eend second nibble */
		lcd_put_command(lcd, aux & 0x0F);
	}
}
