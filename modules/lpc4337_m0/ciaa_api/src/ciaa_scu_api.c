/*
 * ciaa_scu_api.c
 *
 *  Created on: Jun 8, 2022
 *      Author: fabri
 */

#include "ciaa_scu_api.h"

scu_pin_functions_t P2_0 = { .USART = SCU_MODE_FUNC0, .GPIO = SCU_MODE_FUNC4 };
scu_pin_functions_t P2_1 = { .USART = SCU_MODE_FUNC0, .GPIO = SCU_MODE_FUNC4 };
scu_pin_functions_t P2_2 = { .USART = SCU_MODE_FUNC0, .GPIO = SCU_MODE_FUNC4 };
scu_pin_functions_t P2_10 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC2 };
scu_pin_functions_t P2_11 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC2 };
scu_pin_functions_t P2_12 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC7 };

scu_pin_functions_t P1_0 = { .GPIO = SCU_MODE_FUNC0, .SSP = SCU_MODE_FUNC5 };
scu_pin_functions_t P1_1 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .SSP = SCU_MODE_FUNC5 };
scu_pin_functions_t P1_2 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .SSP = SCU_MODE_FUNC5 };
scu_pin_functions_t P1_6 = { .GPIO = SCU_MODE_FUNC0 };

scu_pin_functions_t P6_1 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC2, .I2S = SCU_MODE_FUNC3 };
scu_pin_functions_t P6_4 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC2 };
scu_pin_functions_t P6_5 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC2 };
scu_pin_functions_t P6_7 = { .USB = SCU_MODE_FUNC3, .GPIO = SCU_MODE_FUNC4 };
scu_pin_functions_t P6_8 = { .USB = SCU_MODE_FUNC3, .GPIO = SCU_MODE_FUNC4 };
scu_pin_functions_t P6_9 = { .GPIO = SCU_MODE_FUNC0 };
scu_pin_functions_t P6_10 = { .GPIO = SCU_MODE_FUNC0 };
scu_pin_functions_t P6_11 = { .GPIO = SCU_MODE_FUNC0 };
scu_pin_functions_t P6_12 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1 };

scu_pin_functions_t P4_0 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC6 };
scu_pin_functions_t P4_1 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC6 };
scu_pin_functions_t P4_2 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC6 };
scu_pin_functions_t P4_3 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC6 };
scu_pin_functions_t P1_5 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USB = SCU_MODE_FUNC4, .SSP = SCU_MODE_FUNC5 };
scu_pin_functions_t P7_4 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1 };
scu_pin_functions_t P7_5 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1 };

scu_pin_functions_t P4_4 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC6 };
scu_pin_functions_t P4_5 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1 };
scu_pin_functions_t P4_6 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1 };
scu_pin_functions_t P4_10 = { .GPIO = SCU_MODE_FUNC4, .CAN = SCU_MODE_FUNC6 };
scu_pin_functions_t P4_8 = { .GPIO = SCU_MODE_FUNC4, .CAN = SCU_MODE_FUNC6 };
scu_pin_functions_t P4_9 = { .GPIO = SCU_MODE_FUNC4, .CAN = SCU_MODE_FUNC6 };

scu_pin_functions_t P0_0 = { .GPIO = SCU_MODE_FUNC0, .SSP = SCU_MODE_FUNC1, .I2S = SCU_MODE_FUNC7 };
scu_pin_functions_t P0_1 = { .GPIO = SCU_MODE_FUNC0, .SSP = SCU_MODE_FUNC1, .I2S = SCU_MODE_FUNC7 };
scu_pin_functions_t P1_15 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC1 };
scu_pin_functions_t P1_16 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC1 };
scu_pin_functions_t P1_17 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC1, .CAN = SCU_MODE_FUNC5 };
scu_pin_functions_t P1_18 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC1, .CAN = SCU_MODE_FUNC5 };
scu_pin_functions_t P1_19 = { .SSP = SCU_MODE_FUNC1, .I2S = SCU_MODE_FUNC7 };
scu_pin_functions_t P1_20 = { .GPIO = SCU_MODE_FUNC0, .SSP = SCU_MODE_FUNC1 };
scu_pin_functions_t P7_7 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1 };

scu_pin_functions_t P2_3 = { .I2C = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC2, .GPIO = SCU_MODE_FUNC4, .USB = SCU_MODE_FUNC7 };
scu_pin_functions_t P2_4 = { .I2C = SCU_MODE_FUNC1, .USART = SCU_MODE_FUNC2, .GPIO = SCU_MODE_FUNC4, .USB = SCU_MODE_FUNC7 };

scu_pin_functions_t P6_2 = { .GPIO = SCU_MODE_FUNC0, .USART = SCU_MODE_FUNC2, .I2S = SCU_MODE_FUNC3 };
scu_pin_functions_t P9_5 = { .USB = SCU_MODE_FUNC2, .GPIO = SCU_MODE_FUNC4, .USART = SCU_MODE_FUNC7 };
scu_pin_functions_t P9_6 = { .GPIO = SCU_MODE_FUNC0, .USB = SCU_MODE_FUNC2, .USART = SCU_MODE_FUNC7 };

scu_pin_functions_t P3_1 = { .I2S = SCU_MODE_FUNC0, .CAN = SCU_MODE_FUNC2, .USB = SCU_MODE_FUNC3, .GPIO = SCU_MODE_FUNC4 };
scu_pin_functions_t P3_2 = { .I2S = SCU_MODE_FUNC0, .CAN = SCU_MODE_FUNC2, .USB = SCU_MODE_FUNC3, .GPIO = SCU_MODE_FUNC4 };

scu_pin_functions_t PF_4 = { .SSP = SCU_MODE_FUNC0, .I2S = SCU_MODE_FUNC7 };
scu_pin_functions_t P1_3 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USB = SCU_MODE_FUNC4, .SSP = SCU_MODE_FUNC5 };
scu_pin_functions_t P1_4 = { .GPIO = SCU_MODE_FUNC0, .CTOUT = SCU_MODE_FUNC1, .USB = SCU_MODE_FUNC4, .SSP = SCU_MODE_FUNC5 };

scu_pin_functions_t P6_3 = { .GPIO = SCU_MODE_FUNC0, .USB = SCU_MODE_FUNC1 };
scu_pin_functions_t P6_6 = { .GPIO = SCU_MODE_FUNC0, .USB = SCU_MODE_FUNC3 };

