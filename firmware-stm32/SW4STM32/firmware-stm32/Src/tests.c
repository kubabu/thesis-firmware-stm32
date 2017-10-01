/*
 * tests.c
 *
 *  Created on: Oct 1, 2017
 *      Author: kuba
 */
#include "tests.h"


static USART_TypeDef *_results_usart;

char err_msg[64];

static int16_t _failed_tests = 0;


void reset_failed_test_count() {
	_failed_tests = 0;
}

void check_value(float result, float expected_value, char *msg) {
	if(result != expected_value) {
		sprintf(err_msg, "%s failed: expected %f got %f\r\n", msg, expected_value, result);
		TM_USART_Puts(_results_usart, err_msg);
		_failed_tests++;
	}
}


uint16_t run_all_tests(USART_TypeDef *usart) {
	_results_usart = usart;
	reset_failed_test_count();

	_run_dtw_tests();

	_run_rbuf_tests();

	check_value(_failed_tests, 0, "All tests, some");
	reset_failed_test_count();

	return 0;
}

