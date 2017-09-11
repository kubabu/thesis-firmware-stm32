/*
 * dtw_classifier.c
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */

/*
 * nn_classifier.c
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */
#include <stdlib.h>

#include "classifiers.h"


const int SIZE = 50; // idk


float min2(float x, float y) {
	if(x < y) {
		return x;
	} else {
		return y;
	}
}


float min3(float x, float y, float z) {
	if(x < y) {
		return min2(x, z);
	} else {
		return min2(y, z);
	}
}


float sum(float **arr, int16_t size)
{
	float_t sum = 0;

	for(uint16_t i = 0; i < size; ++i) {
		for(uint16_t j = 0; j < size; ++j) {
			sum += arr[i][j];
		}
	}
	return sum;
}


uint16_t cityblock(float* x, float* y, int16_t size) {
	float sum = 0;
	for(int i = 0; i < size; ++i) {
		sum += fabsf(x[i] - y[i]);
	}
	return sum;
}


uint16_t fastdtw(float* x, float* y, int16_t size_p) {
//	D0 = np.zeros(shape=(size + 1, size + 1))
//	D0[0, 1:] = np.inf
//	D0[1:, 0] = np.inf
//	D1 = D0[1:, 1:]
//	D0[1:, 1:] = cdist(x, y, dist)
//	for i in range(size):
//		for j in range(size):
//			D1[i, j] += min(D0[i, j], D0[i, j + 1], D0[i + 1, j])
//	return D1[-1, -1] / sum(D1.shape)

	float D0[SIZE + 1][SIZE + 1];
	float D1[SIZE][SIZE];
	float inf = 1.0 / 0.0; // xD totally correct carry on
	int size = SIZE;

	for(int16_t i = 1; i <= size; ++i) {
		D0[0][i] = inf;
		D0[i][0] = inf;
	}

	for(int16_t i = 1; i < size; ++i) {
		for(uint16_t j = 1; j < size; ++j) {
			D1[i][j] = cityblock(x, y, size);
		}
	}


	for(uint16_t i = 0; i < size; ++i) {
		for(uint16_t j = 0; j < size; ++j) {
			D1[i][j] += min3(D0[i][j], D0[i][j + 1], D0[i + 1][j]);
		}
	}

	return D1[size][size] / sum((float**)D1, size);
}

