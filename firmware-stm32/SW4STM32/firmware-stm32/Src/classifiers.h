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


// ile ficzeow maja dane (3xacc + 3xgyro + 3xyawpithroll + 3xyawpithrollkalman)
#define FEATURES 12
// jak dluga jest sekwejca (2s * 25HZ = 50)
#define SEQUENCE_LEN 50

//out
#define DTW_SIZE 2 // idk


uint16_t run_nn_classifier(float** data_series);


float fastdtw(float x[][DTW_SIZE], float y[][DTW_SIZE]);

float cityblock(float* x, float* y, int16_t size);


#endif /* CLASSIFIERS_H_ */
