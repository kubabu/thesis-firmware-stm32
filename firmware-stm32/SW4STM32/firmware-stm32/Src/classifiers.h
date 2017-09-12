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

#define DTW_SIZE 50 // idk


uint16_t run_nn_classifier(float** data_series);


float fastdtw(float** x, float** y, int16_t size_p);

float cityblock(float* x, float* y, int16_t size);


#endif /* CLASSIFIERS_H_ */
