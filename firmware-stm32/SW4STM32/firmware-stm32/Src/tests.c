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

// test is failed if test is not true
tests_result_t check_value(int testval, float result, float expected_value, char *msg) {
	if(!testval) {
		sprintf(err_msg, "%s failed: expected %f got %f\r\n", msg, expected_value, result);
		TM_USART_Puts(_results_usart, err_msg);
		_failed_tests++;

		return TEST_FAILED;
	}

	return TEST_PASSED;
}


tests_result_t check_exact_value(float result, float expected_value, char *msg) {
	return check_value(result == expected_value, result, expected_value, msg);
}


uint16_t run_all_tests(USART_TypeDef *usart) {
	_results_usart = usart;
	reset_failed_test_count();

	_run_dtw_tests();
	_run_rbuf_tests();

	if(check_exact_value(_failed_tests, 0, "All tests, some") == TEST_PASSED) {
		TM_USART_Puts(_results_usart, "All tests passed\r\n");
	}
	reset_failed_test_count();

	return 0;
}

