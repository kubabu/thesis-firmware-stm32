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

//#define UTEST_LOC  __function__


typedef enum tests_result_t {
	TEST_FAILED = 0,
	TEST_PASSED = 1
} tests_result_t;


uint16_t run_all_tests(USART_TypeDef *usart);

// test is failed if testval is not true
tests_result_t check_value(int testval, float result, float expected_value, const char *msg);
tests_result_t check_exact_value(float result, float expected_value, const char *msg);

void _run_knn_tests(void);
void _run_dtw_tests(void);
void _run_rbuf_tests(void);
void _run_dataset_tests(void);

void fill_array(float *array, float value, size_t size);

#endif /* TESTS_H_ */
