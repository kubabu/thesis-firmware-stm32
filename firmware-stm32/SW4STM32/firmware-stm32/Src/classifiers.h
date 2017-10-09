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

#define NO_GESTURE_DETECTED -1

// for fastdtw
#define DTW_SEQUENCE_LEN SEQ_LEN
#define DTW_FEATURES FEATURES

int16_t run_nn_classifier(rbuf_iterator_t X[DTW_FEATURES]);

int16_t run_dtw_classifier(float X[DTW_FEATURES][DTW_SEQUENCE_LEN]);

float cityblock(const float x[DTW_SEQUENCE_LEN], float y[DTW_SEQUENCE_LEN]);
float fastdtw(const float x[DTW_FEATURES][DTW_SEQUENCE_LEN], float y[DTW_FEATURES][DTW_SEQUENCE_LEN]);

#endif /* CLASSIFIERS_H_ */
