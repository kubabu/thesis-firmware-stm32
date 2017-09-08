/*
 * defines.h
 *
 *  Created on: Aug 15, 2017
 *      Author: kuba
 */

#ifndef __DEFINES_H_
#define __DEFINES_H_

// link TM libs to used HAL
#include "stm32fxxx_hal.h"

#define USART_MODE_TX_RX 	UART_MODE_TX_RX
#define USART_PARITY_NONE	UART_PARITY_NONE
#define USART_STOPBITS_1	UART_STOPBITS_1
#define USART_FLAG_TXE		UART_FLAG_TXE

#endif /* TM_DEFINES_H_ */
