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


extern float series[FEATURES][PADDED_SEQ_LEN];

extern rbuf_iterator_t nn_iterators[FEATURES];
extern rbuf_iterator_t dtw_iterators[FEATURES];

void classifiers_dataset_init();
void classifiers_dataset_push(IMU_Results *results);


#endif /* CLASSIFIERS_DATA_H_ */
