/*
 * tests.h
 *
 *  Created on: Sep 11, 2017
 *      Author: kuba
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <stdint.h>
#include <stdio.h>
#include <tm_stm32_usart.h>

#include "defines.h"


uint16_t run_all_tests(USART_TypeDef *usart);

void check_value(float result, float expected_value, char *msg);

void _run_dtw_tests(void);
void _run_rbuf_tests(void);

#endif /* TESTS_H_ */
