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


// data features count (3xacc + 3xgyro + 3xyawpithroll + 3xyawpithrollkalman)
#define FEATURES 12
// length of reads window sequence (2s * 25HZ = 50)
#define SEQUENCE_LEN 50
#define PADDING 2
#define PADDED_SEQ_LEN PADDING+SEQUENCE_LEN+PADDING


extern float series[FEATURES][PADDED_SEQ_LEN];

int16_t run_nn_classifier(float data_series[][PADDED_SEQ_LEN]);

//int16_t run_dtw_classifier();


#endif /* CLASSIFIERS_H_ */
