/*
 * ciaa_multicore_api.c
 *
 *  Created on: Jun 16, 2022
 *      Author: fabri
 */

#include "ciaa_multicore_api.h"

/* Pointer array to interrupt handlers */
void (*multicore_irq_handler[])(void) = { NULL, NULL };
