/*
 * classifiers_dataset_tests.c
 *
 *  Created on: Oct 7, 2017
 *      Author: kuba
 */

#include "classifiers_data.h"
#include "tests.h"


void dataset_tests(void) {
	classifiers_dataset_t dataset;
	IMU_Results results;

	dataset_init(&dataset);

	for (int i = 0; i < FEATURES; ++i) {
		check_value(dataset.buffers[i].buffer == dataset.series[i], 0, 0, __FUNCTION__);
		check_value(&dataset.buffers[i] == dataset.dtw_iterators[i].buf, 0, 0, __FUNCTION__);
		check_value(&dataset.buffers[i] == dataset.nn_iterators[i].buf,  0, 0,__FUNCTION__);
	}

	results.ax = 0.0;
	results.ax = 0.0;
	results.ay = 1.0;
	results.az = 2.0;
	results.gx = 3.0;
	results.gy = 4.0;
	results.gz = 5.0;
	results.raw_angles.pitch = 6.0;
	results.raw_angles.roll = 7.0;
	results.raw_angles.yaw = 8.0;
	results.filtered_angles.pitch = 9.0;
	results.filtered_angles.roll = 10.0;
	results.filtered_angles.yaw = 11.0;

	dataset_push(&dataset, &results);

	check_exact_value(ringbuf_peek(&dataset.buffers[0]), 0.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[1]), 1.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[2]), 2.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[3]), 3.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[4]), 4.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[5]), 5.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[6]), 6.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[7]), 7.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[8]), 8.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[9]), 9.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[10]), 10.0, __FUNCTION__);
	check_exact_value(ringbuf_peek(&dataset.buffers[11]), 11.0, __FUNCTION__);
}


void _run_dataset_tests(void) {
	dataset_tests();
}
