/*
 * ringbuf_tests.c
 *
 *  Created on: Oct 1, 2017
 *      Author: kuba
 */

#include "ringbuf.h"
#include "tests.h"

#define TEST_BUF_SIZE 8



void push_tests(void) {
	float buffer[TEST_BUF_SIZE] = { 42.0 };

	ringbuf_t buf = init(TEST_BUF_SIZE, buffer);

	check_value(buf.buffer == buffer, 0, 0, "Buffer assign at init");
	check_exact_value(buf.capacity, TEST_BUF_SIZE, "Cap assign at init");
	check_exact_value(buf.is_filled, 0, "Is filled assign at init");
	check_exact_value(buf.head_index, 0, "Head index at init");
	check_exact_value(buf.buffer[0], 42, "Checking 1st element in buffer");

	check_exact_value(buf.is_filled, 0, "filling check before cycling over");

	for (int i = 0; i < buf.capacity; ++i) {
		push(&buf, i);
		check_exact_value(buffer[i % TEST_BUF_SIZE], i, "Checking n-th added element to buffer");
	}

	check_exact_value(buf.is_filled, 1, "filling check after cycling over");

	for (int i = buf.capacity; i < buf.capacity * 2; ++i) {
		push(&buf, i);
		check_exact_value(buffer[i % TEST_BUF_SIZE], i, "Checking n-th added element to buffer");
	}
}


void _run_rbuf_tests(void) {
	push_tests();
}
