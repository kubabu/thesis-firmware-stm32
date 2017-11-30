/*
 * classifiers_data.c
 *
 *  Created on: Oct 3, 2017
 *      Author: kuba
 */
#include <classifiers_dataset.h>

void dataset_init(classifiers_dataset_t *dataset) {
	dataset->is_ready = DATASET_NOT_READY;
	dataset->count = 0;
}


void dataset_push(classifiers_dataset_t *dataset, IMU_Results *results) {
	const size_t bytes_to_shift = (PADDED_SEQ_LEN - 1) * sizeof(float);

	for (int i = 0; i < FEATURES; ++i) {
		memmove(dataset->series[i], &dataset->series[i][1], bytes_to_shift);
	}

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
		dataset->is_ready = DATASET_READY;
	}
}
