/*
 * classifiers_data.c
 *
 *  Created on: Oct 3, 2017
 *      Author: kuba
 */
#include "classifiers_data.h"


void dataset_init(classifiers_dataset_t *dataset) {
	const size_t buffer_size = PADDED_SEQ_LEN;
	const size_t buffer_count = FEATURES;

	for (int i = 0; i < buffer_count; ++i) {
		dataset->buffers[i] = ringbuf(buffer_size, dataset->series[i]);

		dataset->nn_iterators[i] = get_iterator(&dataset->buffers[i], PADDED_SEQ_LEN);
		dataset->dtw_iterators[i] = get_iterator(&dataset->buffers[i], SEQUENCE_LEN);
	}
}


void dataset_push(classifiers_dataset_t *dataset, IMU_Results *results) {
	ringbuf_push(dataset->buffers + 0, results->ax);
	ringbuf_push(dataset->buffers + 1, results->ay);
	ringbuf_push(dataset->buffers + 2, results->az);
	ringbuf_push(dataset->buffers + 3, results->gx);
	ringbuf_push(dataset->buffers + 4, results->gy);
	ringbuf_push(dataset->buffers + 5, results->gz);
	ringbuf_push(dataset->buffers + 6, results->raw_angles.pitch);
	ringbuf_push(dataset->buffers + 7, results->raw_angles.roll);
	ringbuf_push(dataset->buffers + 8, results->raw_angles.yaw);
	ringbuf_push(dataset->buffers + 9, results->filtered_angles.pitch);
	ringbuf_push(dataset->buffers + 10, results->filtered_angles.roll);
	ringbuf_push(dataset->buffers + 11, results->filtered_angles.yaw);
}


//	float *arrs[4] = { arr[0], arr[1], arr[2], arr[3] };
//
//	ringbufs_init(arrs, bufs, 5, 4);
//
//	for (int i = 0; i < 4; ++i) {
//		ringbuf_t buf = bufs[i];
//		check_exact_value(arr[i] == buf.buffer, 1, __FUNCTION__);
//		check_exact_value(5, buf.capacity, __FUNCTION__);
//	}
