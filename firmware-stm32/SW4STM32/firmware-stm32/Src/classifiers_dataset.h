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


#define DATASET_QUEUE_CAPACITY (SERIAL_READS_UPDATE_FREQUENCY_HZ / DATASET_UPDATE_FREQUENCY_HZ) + 1

typedef enum dataset_state {
	DATASET_NOT_READY = 0,
	DATASET_READY = 1
} dataset_state_t;

typedef struct classifiers_dataset_t {
	float series[FEATURES][PADDED_SEQ_LEN];
	uint8_t count;
	dataset_state_t is_ready;
	IMU_Reads_union queue[DATASET_QUEUE_CAPACITY];
	size_t queue_size;
} classifiers_dataset_t;


/* Dataset with queue usage
 *
 * dataset =  Dataset_Initialize();
 *
 * while(!dataset_processing_finished) {
 *  	process_dataset()
 *  	Dataset_queue_Push(datasetdo_new_reads)
 * }
 * Dataset_queue_Process(dataset);
 */

classifiers_dataset_t Dataset_Initialize();

// add single read to dataset
void Dataset_Push(classifiers_dataset_t *dataset, IMU_Results *results);

// add read to queue, while dataset is processed
void Dataset_queue_Push(classifiers_dataset_t *dataset, IMU_Reads_union *reads);

// eat queue when current dataset processing cycle is finished
void Dataset_queue_Process(classifiers_dataset_t *dataset);


#endif /* CLASSIFIERS_DATASET_H_ */
