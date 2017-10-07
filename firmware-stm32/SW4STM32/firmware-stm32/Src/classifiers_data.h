/*
 * classifiers_data.h
 *
 *  Created on: Oct 3, 2017
 *      Author: kuba
 */

#ifndef CLASSIFIERS_DATA_H_
#define CLASSIFIERS_DATA_H_

#include "classifiers.h"
#include "imu_sensor.h"
#include "ringbuf.h"


typedef struct classifiers_dataset_t {
	float series[FEATURES][PADDED_SEQ_LEN];
	ringbuf_t buffers[FEATURES];
	rbuf_iterator_t nn_iterators[FEATURES];
	rbuf_iterator_t dtw_iterators[FEATURES];
} classifiers_dataset_t;


void dataset_init(classifiers_dataset_t *dataset);
void dataset_push(classifiers_dataset_t *dataset, IMU_Results *results);


#endif /* CLASSIFIERS_DATA_H_ */
