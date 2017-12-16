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

#include "settings.h"


#define PADDING 2
#define NN_SEQ_LEN SEQ_LEN
// for fastdtw
#define DTW_SEQUENCE_LEN NN_SEQ_LEN


void knn_normalize(float *values, float *results);
const char* knn_get_name(int16_t code);
int16_t knn_classifier(const float X[FEATURES][DTW_SEQUENCE_LEN]);

void nn_normalize(float *values, float *results);
const char* nn_get_name(int16_t code);
int16_t nn_classifier(const float series[FEATURES][NN_SEQ_LEN]);



float cityblock(const float x[DTW_SEQUENCE_LEN], const float y[DTW_SEQUENCE_LEN]);
float fastdtw(const float x[FEATURES][DTW_SEQUENCE_LEN],const float y[FEATURES][DTW_SEQUENCE_LEN]);


#endif /* CLASSIFIERS_H_ */
