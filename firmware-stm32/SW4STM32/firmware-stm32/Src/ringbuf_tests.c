/*
 * ringbuf_tests.c
 *
 *  Created on: Oct 1, 2017
 *      Author: kuba
 */

#include "ringbuf.h"
#include "tests.h"

#define TEST_BUF_SIZE 8

void _run_rbuf_tests(void) {
	float buffer[TEST_BUF_SIZE] = { 0.0 };

	ringbuf_t buff;
	buff.capacity = TEST_BUF_SIZE;
	buff.buffer = buffer;


//	for()

}
