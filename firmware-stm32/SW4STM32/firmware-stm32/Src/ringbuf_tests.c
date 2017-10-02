/*
 * ringbuf_tests.c
 *
 *  Created on: Oct 1, 2017
 *      Author: kuba
 */

#include "ringbuf.h"
#include "tests.h"

#define TEST_BUF_SIZE 8


void ringbuf_push_tests(void) {
	float buffer[TEST_BUF_SIZE] = { 42.0 };
	ringbuf_t buf = ringbuf_init(TEST_BUF_SIZE, buffer);

	check_value(buf.buffer == buffer, 0, 0, "Buffer assign at init");
	check_exact_value(buf.capacity, TEST_BUF_SIZE, "Cap assign at init");
	check_exact_value(buf.is_filled, 0, "Is filled assign at init");
	check_exact_value(buf.head_index, -1, "Head index at init");
	check_exact_value(buf.buffer[0], 42, "Checking 1st element in buffer");

	for (int i = 0; i < buf.capacity * 2; ++i) {
		ringbuf_push(&buf, i);
		check_exact_value(buffer[i % TEST_BUF_SIZE], i, "Checking n-th added element to buffer");
		if(i < buf.capacity - 1) {
			check_exact_value(buf.is_filled, 0, "filling check before cycling over");
		} else {
			check_exact_value(buf.is_filled, 1, "filling check after cycling over");
		}
	}
}


void ringbuf_get_prev_tests(void) {
	float buffer[TEST_BUF_SIZE] = { 42.0 };
	ringbuf_t buf = ringbuf_init(TEST_BUF_SIZE, buffer);

	check_exact_value(-9 % TEST_BUF_SIZE, -1, "yyy");

	for (int16_t i = 0; i < buf.capacity * 2; ++i) {
		ringbuf_push(&buf, i);

		check_exact_value(ringbuf_peek(&buf), i, "Peeking element added to buffer");
		check_exact_value(ringbuf_get_prev(&buf, 0), i, "Getting last added element from buffer");

		if(i >= buf.capacity) {
			for (int16_t j = 0; j > -TEST_BUF_SIZE; --j) {
				check_exact_value(ringbuf_get_prev(&buf, j), i + j, "Getting prev element from buffer");
			}
		}
	}
}


void ringbuf_iterate_tests(void) {
	const int16_t iter_size = 4;
	const int16_t iter_size2 = 6;
	int repeats = 0;

	float buffer[TEST_BUF_SIZE];
	ringbuf_t buf = ringbuf_init(TEST_BUF_SIZE, buffer);
	for (int i = 0; i < buf.capacity; ++i) {
		ringbuf_push(&buf, i);
	}
	rbuf_iterator_t iter = get_iterator(&buf, iter_size);

	check_value(iter.buf->buffer == buffer, 0, 0, "Buffer assign at iterator init");
	check_exact_value(iter.size, iter_size, "iterator size set");

	// iterate over last added 4 elements
	do {
		for (int i = 0; i < iter.size; ++i) {
			check_exact_value(iterate(&iter, i), buf.capacity - iter_size + i, "checking first iteration");
		}
		repeats++;
		// should be idempotent
	} while(repeats < 2);

	// check with another iterator on the same buffer
	rbuf_iterator_t iter2 = get_iterator(&buf, iter_size2);
	for (int i = 0; i < iter2.size; ++i) {
		check_exact_value(iterate(&iter2, i), buf.capacity - iter_size2 + i, "iterator size set");
	}

	// push one more element
	float nextval = ringbuf_peek(&buf) + 1;
	ringbuf_push(&buf, nextval);

	// check if it still works
	for (int i = 0; i < iter.size; ++i) {
		check_exact_value(iterate(&iter, i), iter_size + i + 1, "checking iteration after push");
	}
}


void get_iterator3_tests(void) {
	const int16_t iter_size = 4;
	int repeats = 0;

	float buffer[TEST_BUF_SIZE];
	rbuf_iterator_t iter = get_iterator3(buffer, TEST_BUF_SIZE, iter_size);

	for (int i = 0; i < iter.buf->capacity; ++i) {
		ringbuf_push(iter.buf, i);
	}

	check_value(iter.buf->buffer == buffer, 0, 0, "Buffer assign  in get_iterator3");
	check_exact_value(iter.size, iter_size, "iterator size set in get_iterator3");

	// iterate over last added 4 elements
	do {
		for (int i = 0; i < iter.size; ++i) {
			check_exact_value(iterate(&iter, i), iter.buf->capacity - iter_size + i,
					"checking iterator with auto generated buffer");
		}
		repeats++;
		// should be idempotent
	} while(repeats < 2);
}


void iterator_on_entire_buffer_tests(void) {
	const int16_t iter_size = TEST_BUF_SIZE;
	int repeats = 0;

	float buffer[TEST_BUF_SIZE];
	rbuf_iterator_t iter = get_iterator3(buffer, TEST_BUF_SIZE, iter_size);

	for (int i = 0; i < iter.buf->capacity; ++i) {
		ringbuf_push(iter.buf, i);
	}

	check_value(iter.buf->buffer == buffer, 0, 0, "Buffer assign in iter on_entire_buffer");
	check_exact_value(iter.size, iter_size, "iterator size set in iter on_entire_buffer");

	// iterate over last added 4 elements
	do {
		for (int i = 0; i < iter.size; ++i) {
			check_exact_value(iterate(&iter, i), iter.buf->capacity - iter_size + i,
					"checking iterator on entire buffer");
		}
		repeats++;
		// should be idempotent
	} while(repeats < 2);
}


void _run_rbuf_tests(void) {
	ringbuf_push_tests();
	ringbuf_get_prev_tests();
	ringbuf_iterate_tests();
	get_iterator3_tests();
	iterator_on_entire_buffer_tests();
}
