/*
 * classifiers_data.c
 *
 *  Created on: Oct 3, 2017
 *      Author: kuba
 */
#include <classifiers_dataset.h>


classifiers_dataset_t Dataset_Initialize() {
	classifiers_dataset_t dataset;
	dataset.is_ready = DATASET_NOT_READY;
	dataset.count = 0;
	dataset.queue_size = 0;
	return dataset;
}


// TODO if queue processing is too slow, entire queue can be pushed
void Dataset_Push(classifiers_dataset_t *dataset, IMU_Results *results) {
	const size_t bytes_to_shift = (PADDED_SEQ_LEN - 1) * sizeof(float);
	// push over previous results: first in first out
	for (int i = 0; i < FEATURES; ++i) {
		memmove(dataset->series[i], &dataset->series[i][1], bytes_to_shift);
	}
	// at the end insert fresh results
	dataset->series[0][PADDED_SEQ_LEN - 1] = results->ax;
	dataset->series[1][PADDED_SEQ_LEN - 1] = results->ay;
	dataset->series[2][PADDED_SEQ_LEN - 1] = results->az;
	dataset->series[3][PADDED_SEQ_LEN - 1] = results->gx;
	dataset->series[4][PADDED_SEQ_LEN - 1] = results->gy;
	dataset->series[5][PADDED_SEQ_LEN - 1] = results->gz;
	dataset->series[6][PADDED_SEQ_LEN - 1] = results->raw_angles.pitch;
	dataset->series[7][PADDED_SEQ_LEN - 1] = results->raw_angles.roll;
	dataset->series[8][PADDED_SEQ_LEN - 1] = results->raw_angles.yaw;
	dataset->series[9][PADDED_SEQ_LEN - 1] = results->imu_angles.pitch;
	dataset->series[10][PADDED_SEQ_LEN - 1] = results->imu_angles.roll;
	dataset->series[11][PADDED_SEQ_LEN - 1] = results->imu_angles.yaw;

	++dataset->count;
	if(dataset->count >= PADDED_SEQ_LEN) {
		// first read frame is filled, data in set can be used
		dataset->is_ready = DATASET_READY;
	}
}


void Dataset_queue_Push(classifiers_dataset_t *dataset, IMU_Reads_union *results) {
	// push over previous results: first in first out
	size_t bytes_to_move = sizeof(IMU_Reads_union) * dataset->queue_size;
	if(dataset->queue_size == DATASET_QUEUE_CAPACITY) {
		bytes_to_move = sizeof(IMU_Reads_union) * (DATASET_QUEUE_CAPACITY - 1);
	}
	memmove(dataset->queue+1, dataset->queue, bytes_to_move);
	// add new results
	dataset->queue[0] = *results;
	if(dataset->queue_size < DATASET_QUEUE_CAPACITY) {
		++dataset->queue_size;
	}
}


void Dataset_queue_Process(classifiers_dataset_t *dataset) {
	// TODO this might be too slow
	for (int i = dataset->queue_size - 1; i >= 0; --i) {
		Dataset_Push(dataset, &dataset->queue[i].results);
	}
	dataset->queue_size = 0;
}
