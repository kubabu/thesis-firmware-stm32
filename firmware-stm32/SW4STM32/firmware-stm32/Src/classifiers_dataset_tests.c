/*
 * classifiers_dataset_tests.c
 *
 *  Created on: Oct 7, 2017
 *      Author: kuba
 */

#include <classifiers_dataset.h>
#include "tests.h"

void dataset_init_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();

	check_exact_value(dataset.is_ready, DATASET_NOT_READY, __FUNCTION__);
	check_exact_value(dataset.count, 0, __FUNCTION__);

	IMU_Results results;

	for (int i = 0; i < PADDED_SEQ_LEN; ++i) {
		Dataset_Push(&dataset, &results);
	}

	check_exact_value(dataset.is_ready, DATASET_READY, __FUNCTION__);
}

void dataset_push_tests(void) {
	uint32_t start, duration;
	classifiers_dataset_t dataset = Dataset_Initialize();

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


void dataset_push_order_tests(void) {
	uint32_t start, duration;
	classifiers_dataset_t dataset = Dataset_Initialize();

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

	IMU_Reads_union reads;
	reads.results.ax = 0.123;
	reads.results.ay = 1.123;
	reads.results.az = 2.123;
	reads.results.gx = 3.123;
	reads.results.gy = 4.123;
	reads.results.gz = 5.123;
	reads.results.raw_angles.pitch = 6.123;
	reads.results.raw_angles.roll = 7.123;
	reads.results.raw_angles.yaw = 8.123;
	reads.results.imu_angles.pitch = 9.123;
	reads.results.imu_angles.roll = 10.123;
	reads.results.imu_angles.yaw = 11.123;
	check_exact_value(dataset.is_ready, DATASET_NOT_READY, __FUNCTION__);

	start = HAL_GetTick();

	// act
	Dataset_Push(&dataset, &reads.results);

	//	verify
	uint8_t last_index = PADDED_SEQ_LEN - 1;
	duration = HAL_GetTick() - start;
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < last_index; ++j) {
			check_exact_value(dataset.series[i][j], (j + 1) * i, __FUNCTION__);
		}
		check_exact_value(dataset.series[i][last_index], reads.buffer[i], __FUNCTION__);
	}
	check_value(duration <= 1, duration, 1, __FUNCTION__);
	check_exact_value(dataset.is_ready, DATASET_NOT_READY, __FUNCTION__);
}



void dataset_queue_push_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union results;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	results.buffer[0] = 1;
	results.buffer[1] = 2.0;
	results.buffer[3] = 42.0;

	// act
	Dataset_queue_Push(&dataset, &results);

	// verify: pushed value
	check_exact_value(dataset.queue_size, 1, __FUNCTION__);
	for (int j = 0; j < FEATURES; ++j) {
		check_exact_value(dataset.queue[0].buffer[j], results.buffer[j], __FUNCTION__);
	}
	// main dataset should be untouched
	check_exact_value(dataset.count, 0, __FUNCTION__);
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			check_exact_value(dataset.series[i][j], 0, __FUNCTION__);
		}
	}
}


void dataset_queue_push_2_queue_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union reads;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	reads.buffer[0] = 0;
	reads.buffer[1] = 2.0;
	reads.buffer[3] = 42.0;

	IMU_Reads_union reads2 = reads;
	reads2.buffer[0] = 1.0;
	reads2.buffer[1] = 2.0;
	reads.buffer[3] = 69.0;
	// act
	Dataset_queue_Push(&dataset, &reads);
	Dataset_queue_Push(&dataset, &reads2);

	// verify
	check_exact_value(dataset.queue_size, 2, __FUNCTION__);
	for (int j = 0; j < FEATURES; ++j) {
		check_exact_value(dataset.queue[0].buffer[j], reads2.buffer[j], __FUNCTION__);
	}
	for (int j = 0; j < FEATURES; ++j) {
		check_exact_value(dataset.queue[1].buffer[j], reads.buffer[j], __FUNCTION__);
	}

	// main dataset should be untouched
	check_exact_value(dataset.count, 0, __FUNCTION__);
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			check_exact_value(dataset.series[i][j], 0, __FUNCTION__);
		}
	}
}


void dataset_queue_push_3_queue_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union reads, reads2, reads3;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	reads.buffer[0] = 0;
	reads.buffer[1] = 2.0;
	reads.buffer[3] = 42.0;

	reads2.buffer[0] = 1.0;
	reads2.buffer[1] = 2.0;
	reads2.buffer[3] = 69.0;

	reads3.buffer[0] = 2.0;
	reads3.buffer[1] = 12.0;
	reads3.buffer[3] = 96.0;
	// act
	Dataset_queue_Push(&dataset, &reads);
	Dataset_queue_Push(&dataset, &reads2);
	Dataset_queue_Push(&dataset, &reads3);

	// verify
	check_exact_value(dataset.queue_size, 3, __FUNCTION__);
	for (int j = 0; j < FEATURES; ++j) {
		check_exact_value(dataset.queue[0].buffer[j], reads3.buffer[j], __FUNCTION__);
	}
	for (int j = 0; j < FEATURES; ++j) {
			check_exact_value(dataset.queue[1].buffer[j], reads2.buffer[j], __FUNCTION__);
		}
	for (int j = 0; j < FEATURES; ++j) {
		check_exact_value(dataset.queue[2].buffer[j], reads.buffer[j], __FUNCTION__);
	}

	// main dataset should be untouched
	check_exact_value(dataset.count, 0, __FUNCTION__);
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			check_exact_value(dataset.series[i][j], 0, __FUNCTION__);
		}
	}
}


void dataset_queue_push_full_queue_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union results;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	results.buffer[0] = 0;
	results.buffer[1] = 2.0;
	results.buffer[3] = 42.0;

	// act
	for (int i = 0; i < DATASET_QUEUE_CAPACITY; ++i) {
		results.buffer[0] = i;
		Dataset_queue_Push(&dataset, &results);
	}

	// verify
	check_exact_value(dataset.queue_size, DATASET_QUEUE_CAPACITY, __FUNCTION__);
	for (int i = 0; i < DATASET_QUEUE_CAPACITY; ++i) {
		check_exact_value(dataset.queue[i].buffer[0], DATASET_QUEUE_CAPACITY - i - 1 , __FUNCTION__);
		for (int j = 1; j < FEATURES; ++j) {
			check_exact_value(dataset.queue[0].buffer[j], results.buffer[j], __FUNCTION__);
		}
	}
	// main dataset should be untouched
	check_exact_value(dataset.count, 0, __FUNCTION__);
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			check_exact_value(dataset.series[i][j], 0, __FUNCTION__);
		}
	}
}


void dataset_queue_push_overflown_queue_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union results;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	results.buffer[0] = 0;
	results.buffer[1] = 2.0;
	results.buffer[3] = 42.0;

	// act
	for (int i = 0; i <= DATASET_QUEUE_CAPACITY; ++i) {
		results.buffer[0] = i;
		Dataset_queue_Push(&dataset, &results);
	}

	// verify
	check_exact_value(dataset.queue_size, DATASET_QUEUE_CAPACITY, __FUNCTION__);
	// FIFO: queue[0] is last added
	for (int i = 0; i < DATASET_QUEUE_CAPACITY; ++i) {
		check_exact_value(dataset.queue[i].buffer[0], DATASET_QUEUE_CAPACITY - i, __FUNCTION__);
		for (int j = 1; j < FEATURES; ++j) {
			check_exact_value(dataset.queue[0].buffer[j], results.buffer[j], __FUNCTION__);
		}
	}
	// main dataset should be untouched
	check_exact_value(dataset.count, 0, __FUNCTION__);
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			check_exact_value(dataset.series[i][j], 0, __FUNCTION__);
		}
	}
}


void dataset_queue_process_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union results;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	results.buffer[0] = 1;
	results.buffer[1] = 2.0;
	results.buffer[3] = 42.0;
	Dataset_queue_Push(&dataset, &results);
	check_exact_value(dataset.queue_size, 1, __FUNCTION__);

	// act
	Dataset_queue_Process(&dataset);

	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	check_exact_value(dataset.count, 1, __FUNCTION__);
	for (int i = 0; i < FEATURES; ++i) {
		check_exact_value(dataset.series[i][PADDED_SEQ_LEN - 1], results.buffer[i], __FUNCTION__);
	}
}


void dataset_queue_process_multiple_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union reads;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	reads.buffer[0] = 1;
	reads.buffer[1] = 2.0;
	reads.buffer[3] = 42.0;
	Dataset_queue_Push(&dataset, &reads);
	reads.buffer[0] = 2;
	Dataset_queue_Push(&dataset, &reads);
	reads.buffer[0] = 3;
	Dataset_queue_Push(&dataset, &reads);
	check_exact_value(dataset.queue_size, 3, __FUNCTION__);

	// act
	Dataset_queue_Process(&dataset);

	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	check_exact_value(dataset.count, 3, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 3], 1.0, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 2], 2.0, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 1], 3.0, __FUNCTION__);
	for (int j = 0; j < 3; ++j) {
		check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 3 + j], j + 1, __FUNCTION__);
	}
	for (int i = 1; i < FEATURES; ++i) {
		for (int j = 0; j < 3; ++j) {
			check_exact_value(dataset.series[i][PADDED_SEQ_LEN - 3 + j], reads.buffer[i], __FUNCTION__);
		}
	}
}


void dataset_queue_process_full_queue_tests(void) {
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union reads;
	// initialize with 0s
	for (int i = 0; i < FEATURES; ++i) {
		for (int j = 0; j < PADDED_SEQ_LEN; ++j) {
			dataset.series[i][j] = 0;
		}
	}
	reads.buffer[1] = 2.0;
	reads.buffer[3] = 42.0;
	for (int i = 0; i <= DATASET_QUEUE_CAPACITY; ++i) {
		reads.buffer[0] = i;
		Dataset_queue_Push(&dataset, &reads);
	}
	check_exact_value(dataset.queue_size, DATASET_QUEUE_CAPACITY, __FUNCTION__);

	// act
	Dataset_queue_Process(&dataset);

	check_exact_value(dataset.queue_size, 0, __FUNCTION__);
	check_exact_value(dataset.count, DATASET_QUEUE_CAPACITY, __FUNCTION__);

	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 5], 1.0, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 4], 2.0, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 3], 3.0, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 2], 4.0, __FUNCTION__);
	check_exact_value(dataset.series[0][PADDED_SEQ_LEN - 1], 5.0, __FUNCTION__);

	for (int j = 0; j < DATASET_QUEUE_CAPACITY; ++j) {
		check_exact_value(dataset.series[0][PADDED_SEQ_LEN - DATASET_QUEUE_CAPACITY + j], j + 1, __FUNCTION__);
	}
	for (int i = 1; i < FEATURES; ++i) {
		for (int j = 0; j < DATASET_QUEUE_CAPACITY; ++j) {
			check_exact_value(dataset.series[i][PADDED_SEQ_LEN - DATASET_QUEUE_CAPACITY + j], reads.buffer[i], __FUNCTION__);
		}
	}
}


void benchmark_push_classifier_runtime() {
	uint32_t start, duration;
	char msg[128];
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union reads;
	for (int i = 0; i <= DATASET_QUEUE_CAPACITY; ++i) {
		reads.buffer[0] = i;
		Dataset_queue_Push(&dataset, &reads);
	}
	start = HAL_GetTick();

	Dataset_Push(&dataset, &reads.results);

	duration = HAL_GetTick() - start;
	sprintf(msg, "Benchmarking Dataset_Push: %ld [ms]\r\n", duration);
	TM_USART_Puts(USART6, msg);
	check_value(duration <= CLASSIFIER_UPDATE_INTERVAL_MS, duration, CLASSIFIER_UPDATE_INTERVAL_MS, __FUNCTION__);
}


void benchmark_queue_process_classifier_runtime() {
	uint32_t start, duration;
	char msg[128];
	classifiers_dataset_t dataset = Dataset_Initialize();
	IMU_Reads_union reads;
	for (int i = 0; i <= DATASET_QUEUE_CAPACITY; ++i) {
		reads.buffer[0] = i;
		Dataset_queue_Push(&dataset, &reads);
	}
	start = HAL_GetTick();

	Dataset_queue_Process(&dataset);

	duration = HAL_GetTick() - start;
	sprintf(msg, "Benchmarking Dataset_queue_Process: %ld [ms]\r\n", duration);
	TM_USART_Puts(USART6, msg);
	check_value(duration <= CLASSIFIER_UPDATE_INTERVAL_MS, duration, CLASSIFIER_UPDATE_INTERVAL_MS, __FUNCTION__);
}


void _run_dataset_tests(void) {
	dataset_init_tests();
	dataset_push_tests();
	dataset_push_order_tests();
	dataset_queue_push_tests();
	dataset_queue_push_2_queue_tests();
	dataset_queue_push_3_queue_tests();
	dataset_queue_push_full_queue_tests();
	dataset_queue_push_overflown_queue_tests();
	dataset_queue_process_tests();
	dataset_queue_process_multiple_tests();
	dataset_queue_process_full_queue_tests();

	benchmark_push_classifier_runtime();
	benchmark_queue_process_classifier_runtime();
}
