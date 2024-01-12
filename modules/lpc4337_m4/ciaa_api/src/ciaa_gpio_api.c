/*
 * ciaa_gpio_api.c
 *
 *  Created on: Jun 2, 2022
 *      Author: fabri
 */

/* CIAA GPIO API includes */
#include "ciaa_gpio_api.h"

/* Pin definitions */
pin_t LEDR = { 2, 0, 5, 0, &P2_0 };
pin_t LEDG = { 2, 1, 5, 1, &P2_1 };
pin_t LEDB = { 2, 2, 5, 2, &P2_2 };
pin_t LED1 = { 2, 10, 0, 14, &P2_10 };
pin_t LED2 = { 2, 11, 1, 11, &P2_11 };
pin_t LED3 = { 2, 12, 1, 12, &P2_12 };

pin_t TEC1 = { 1, 0, 0, 4, &P1_0 };
pin_t TEC2 = { 1, 1, 0, 8, &P1_1 };
pin_t TEC3 = { 1, 2, 0, 9, &P1_2 };
pin_t TEC4 = { 1, 6, 1, 9, &P1_6 };

pin_t GPIO0 = { 6, 1, 3, 0, &P6_1 };
pin_t GPIO1 = { 6, 4, 3, 3, &P6_4 };
pin_t GPIO2 = { 6, 5, 3, 4, &P6_5 };
pin_t GPIO3 = { 6, 7, 5, 15, &P6_7 };
pin_t GPIO4 = { 6, 8, 5, 16, &P6_8};
pin_t GPIO5 = { 6, 9, 3, 5, &P6_9 };
pin_t GPIO6 = { 6, 10, 3, 6, &P6_10 };
pin_t GPIO7 = { 6, 11, 3, 7, &P6_11 };
pin_t GPIO8 = { 6, 12, 2, 8, &P6_12 };

pin_t TFIL0 = { 4, 0, 2, 0, &P4_0 };
pin_t TFIL1 = { 4, 1, 2, 1, &P4_1 };
pin_t TFIL2 = { 4, 2, 2, 2, &P4_2 };
pin_t TFIL3 = { 4, 3, 2, 3, &P4_3 };

pin_t TCOL0 = { 1, 5, 1, 8, &P1_5 };
pin_t TCOL1 = { 7, 4, 3, 12, &P7_4 };
pin_t TCOL2 = { 7, 5, 3, 13, &P7_5 };

pin_t LCD1 = { 4, 4, 2, 4, &P4_4 };
pin_t LCD2 = { 4, 5, 2, 5, &P4_5 };
pin_t LCD3 = { 4, 6, 2, 6, &P4_6 };
pin_t LCD4 = { 4, 10, 5, 14, &P4_10 };
pin_t LCD_RS = { 4, 8, 5, 12, &P4_8 };
pin_t LCD_EN = { 4, 9, 5, 13, &P4_9 };

pin_t RXD1 = { 0, 0, 0, 0, &P0_0 };
pin_t TX_EN = { 0, 1, 0, 1, &P0_1 };
pin_t RXD0 = { 1, 15, 0, 2, &P1_15 };
pin_t CRS_DV = { 1, 16, 0, 3, &P1_16 };
pin_t MDIO = { 1, 17, 0, 12, &P1_17 };
pin_t TXD0 = { 1, 18, 0, 13, &P1_18 };
pin_t TXD1 = { 1, 20, 0, 15, &P1_20 };
pin_t MDC = { 7, 7, 3, 15, &P7_7 };

pin_t RS232_TXD = { 2, 3, 5, 3, &P2_3 };
pin_t RS232_RXD = { 2, 4, 5, 4, &P2_4 };

pin_t RS485_DIR = { 6, 2, 3, 1, &P6_2 };
pin_t RS485_TXD = { 9, 5, 5, 18, &P9_5 };
pin_t RS485_RXD = { 9, 6, 4, 11, &P9_6 };

pin_t CAN_RD = { 3, 1, 5, 8, &P3_1 };
pin_t CAN_TD = { 3, 2, 5, 9, &P3_2 };

pin_t SPI_MISO = { 1, 3, 0, 10, &P1_3 };
pin_t SPI_MOSI = { 1, 4, 0, 11, &P1_4 };

