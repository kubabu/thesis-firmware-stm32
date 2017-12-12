/*
 * classifiers_dataset_tests.c
 *
 *  Created on: Oct 7, 2017
 *      Author: kuba
 */

#include <classifiers_dataset.h>
#include "tests.h"

void dataset_init_tests(void) {
	classifiers_dataset_t dataset;

	Dataset_Initialize(&dataset);

	check_exact_value(dataset.is_ready, DATASET_NOT_READY, __FUNCTION__);
	check_exact_value(dataset.count, 0, __FUNCTION__);

	IMU_Results results;

	for (int i = 0; i < PADDED_SEQ_LEN; ++i) {
		Dataset_Push(&dataset, &results);
	}

	check_exact_value(dataset.is_ready, DATASET_READY, __FUNCTION__);
}

void dataset_push_tests(void) {
	classifiers_dataset_t dataset;
	uint32_t start, duration;

	Dataset_Initialize(&dataset);
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = j * i;
		}
	}
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			check_exact_value(dataset.series[i][j], j * i, __FUNCTION__);
		}
	}

	IMU_Results results;
	results.ax = 0.123;
	results.ay = 1.123;
	results.az = 2.123;
	results.gx = 3.123;
	results.gy = 4.123;
	results.gz = 5.123;
	results.raw_angles.pitch = 6.123;
	results.raw_angles.roll = 7.123;
	results.raw_angles.yaw = 8.123;
	results.imu_angles.pitch = 9.123;
	results.imu_angles.roll = 10.123;
	results.imu_angles.yaw = 11.123;
	check_exact_value(dataset.is_ready, DATASET_NOT_READY, __FUNCTION__);

	start = HAL_GetTick();

	// act
	Dataset_Push(&dataset, &results);

	//	verify
	duration = HAL_GetTick() - start;
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN - 1; ++j) {
			check_exact_value(dataset.series[i][j], (j + 1) * i, __FUNCTION__);
		}
	}
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 1], results.ax, __FUNCTION__);
	check_exact_value(dataset.series[1][PADDED_SEQ_LEN - 1], results.ay, __FUNCTION__);
	check_exact_value(dataset.series[2][PADDED_SEQ_LEN - 1], results.az, __FUNCTION__);
	check_exact_value(dataset.series[3][PADDED_SEQ_LEN - 1], results.gx, __FUNCTION__);
	check_exact_value(dataset.series[4][PADDED_SEQ_LEN - 1], results.gy, __FUNCTION__);
	check_exact_value(dataset.series[5][PADDED_SEQ_LEN - 1], results.gz, __FUNCTION__);
	check_exact_value(dataset.series[6][PADDED_SEQ_LEN - 1], results.raw_angles.pitch, __FUNCTION__);
	check_exact_value(dataset.series[7][PADDED_SEQ_LEN - 1], results.raw_angles.roll, __FUNCTION__);
	check_exact_value(dataset.series[8][PADDED_SEQ_LEN - 1], results.raw_angles.yaw, __FUNCTION__);
	check_exact_value(dataset.series[9][PADDED_SEQ_LEN - 1], results.imu_angles.pitch, __FUNCTION__);
	check_exact_value(dataset.series[10][PADDED_SEQ_LEN - 1], results.imu_angles.roll, __FUNCTION__);
	check_exact_value(dataset.series[11][PADDED_SEQ_LEN - 1], results.imu_angles.yaw, __FUNCTION__);
	check_value(duration <= 1, duration, 1, __FUNCTION__);
	check_exact_value(dataset.is_ready, DATASET_NOT_READY, __FUNCTION__);
}

void _run_dataset_tests(void) {
	dataset_init_tests();
	dataset_push_tests();
}
