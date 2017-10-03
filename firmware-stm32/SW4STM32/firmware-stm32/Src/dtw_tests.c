/*
 * dtw_tests.c
 *
 *  Created on: Sep 20, 2017
 *      Author: kuba
 */
#include "classifiers.h"
#include "tests.h"


void cityblock_tests_1(void) {
	float x[DTW_SEQUENCE_LEN] = {0.0};
	float ybuf[DTW_SEQUENCE_LEN] = {0.0};
	rbuf_iterator_t y = get_iterator4(ybuf, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);

	check_exact_value(cityblock(x, y), 0.0, __FUNCTION__);
}


void cityblock_tests_2(void) {
	float x[DTW_SEQUENCE_LEN] = {0.0};
	float ybuf2[DTW_SEQUENCE_LEN] = {
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	rbuf_iterator_t y2 = get_iterator4(ybuf2, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);

	check_exact_value(cityblock(x, y2), 50.0, __FUNCTION__);
}


void fastdtw_tests_0(void) {
	float x[DTW_FEATURES][DTW_SEQUENCE_LEN] = {
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}};

	float *y0 = x[0];
	rbuf_iterator_t it0 = get_iterator4(y0, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);

	rbuf_iterator_t *y0s[DTW_FEATURES];
	for(int i = 0; i < DTW_FEATURES; ++i) {
		y0s[i] = &it0;
	}

	check_exact_value(fastdtw(x, y0s), 0.0, __FUNCTION__);
}


void fastdtw_tests_1(void) {
	float x[DTW_FEATURES][DTW_SEQUENCE_LEN] = {
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}};

	float y1[DTW_SEQUENCE_LEN] = {
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	rbuf_iterator_t it1 = get_iterator4(y1, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);

	rbuf_iterator_t *y1s[DTW_FEATURES];
	for(int i = 0; i < DTW_FEATURES; ++i) {
		y1s[i] = &it1;
	}

	check_exact_value(fastdtw(x, y1s), 25.0, __FUNCTION__);
}


void fastdtw_tests_2(void) {
	float x[DTW_FEATURES][DTW_SEQUENCE_LEN] = {
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}};

	float *y0 = x[0];
	float y1[DTW_SEQUENCE_LEN] = {
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	rbuf_iterator_t it0 = get_iterator4(y0, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);
	rbuf_iterator_t it1 = get_iterator4(y1, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);

	rbuf_iterator_t *y1s[DTW_FEATURES];
	y1s[0] = &it1;
	for(int i = 1; i < DTW_FEATURES; ++i) {
		y1s[i] = &it0;
	}

	check_exact_value(fastdtw(x, y1s), 2.08333325, __FUNCTION__);
}


void fastdtw_tests_3(void) {
	float x[DTW_FEATURES][DTW_SEQUENCE_LEN] = {
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}, {0.0}, {0.0}, {0.0},
			{0.0}, {0.0}};

	float *y0 = x[0];
	float y3[DTW_SEQUENCE_LEN] = {
		1.0, 2.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	rbuf_iterator_t it0 = get_iterator4(y0, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);
	rbuf_iterator_t it3 = get_iterator4(y3, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN, DTW_SEQUENCE_LEN);

	rbuf_iterator_t *y1s[DTW_FEATURES];
	y1s[0] = &it3;
	for(int i = 1; i < DTW_FEATURES; ++i) {
		y1s[i] = &it0;
	}

	check_exact_value(fastdtw(x, y1s), 0.416666657, __FUNCTION__);
}

//	check_exact_value(fastdtw(x, y3), 0.416666657, "fastdtw test y3");



void benchmark_runtimes() {
	char msg[50];
	float X[DTW_FEATURES][DTW_SEQUENCE_LEN] = {0};
	uint32_t start, duration;
	int16_t result;
	const uint16_t nn_expected_time = 3;
	const uint16_t dtw_expected_time = 10;

	start = HAL_GetTick();
	result = run_nn_classifier(series);
	duration = HAL_GetTick() - start;

	sprintf(msg, "Benchmarking NN classifier: result=%d in %ld [ms]\r\n", result, duration);
	TM_USART_Puts(USART6, msg);
	check_value(duration <= nn_expected_time, duration, nn_expected_time,
				"Benchmarking NN classifier execution time");


	start = HAL_GetTick();
	result = run_dtw_classifier(X);
	duration = HAL_GetTick() - start;

	sprintf(msg, "Benchmarking DTW classifier: result=%d in %ld [ms]\r\n", result, duration);
	TM_USART_Puts(USART6, msg);
	check_value(duration <= dtw_expected_time, duration, dtw_expected_time,
			"Benchmarking DTW classifier execution time");
}


void _run_dtw_tests(void) {
	cityblock_tests_1();
	cityblock_tests_2();
	fastdtw_tests_0();
	fastdtw_tests_1();
	fastdtw_tests_2();
	fastdtw_tests_3();

	benchmark_runtimes();
}

