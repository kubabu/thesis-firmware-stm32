/*
 * classifiers.h
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */

#ifndef CLASSIFIERS_H_
#define CLASSIFIERS_H_

#include <math.h>
#include <stdint.h>

#include "ringbuf.h"


// data features count (3xacc + 3xgyro + 3xyawpithroll + 3xyawpithrollkalman)
#define FEATURES 12
// length of reads window sequence (2s * 25HZ = 50)
#define SEQ_LEN 50
#define PADDING 2
#define PADDED_SEQ_LEN PADDING+SEQ_LEN+PADDING
// for fastdtw
#define DTW_SEQUENCE_LEN PADDED_SEQ_LEN

#define NO_GESTURE_DETECTED -1

void knn_normalize(float *values, float *results);
const char* knn_get_name(int16_t code);
int16_t knn_classifier(const float X[FEATURES][DTW_SEQUENCE_LEN]);

void nn_normalize(float *values, float *results);
const char* nn_get_name(int16_t code);
int16_t nn_classifier(const float series[FEATURES][PADDED_SEQ_LEN]);



float cityblock(const float x[DTW_SEQUENCE_LEN], const float y[DTW_SEQUENCE_LEN]);
float fastdtw(const float x[FEATURES][DTW_SEQUENCE_LEN],const float y[FEATURES][DTW_SEQUENCE_LEN]);


#endif /* CLASSIFIERS_H_ */
