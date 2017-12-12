/*
 * classifiers_data.h
 *
 *  Created on: Oct 3, 2017
 *      Author: kuba
 */

#ifndef CLASSIFIERS_DATASET_H_
#define CLASSIFIERS_DATASET_H_

#include <imu.h>
#include "classifiers.h"
#include "ringbuf.h"

typedef enum dataset_state {
	DATASET_NOT_READY = 0,
	DATASET_READY = 1
} dataset_state_t;

typedef struct classifiers_dataset_t {
	float series[FEATURES][PADDED_SEQ_LEN];
	uint8_t count;
	dataset_state_t is_ready;
	// TODO queue, imu?
} classifiers_dataset_t;


void Dataset_Initialize(classifiers_dataset_t *dataset);

void Dataset_Push(classifiers_dataset_t *dataset, IMU_Results *results);

// for use in interrupts
void Dataset_queue_Push(classifiers_dataset_t *dataset, IMU_Results_t *results);


#endif /* CLASSIFIERS_DATASET_H_ */
