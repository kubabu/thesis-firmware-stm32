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
#include "ringbuf.h"


// ile sasiadow brac, to samo co self.k
#define K 3
// powyzej jakiego ksotu dopasowania uznajemy ze nic z tego nie bedzie (self.threshold)
#define THRESHOLD 100
// ile gestow mamy zdefiniowane w sumie
#define NUMBER_OF_GESTURES 13
// ile jest zapamietanych probek. to samo co w metodze fit gdyby napisac X.shape[0]
#define BATCH_SIZE 11


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


float sum(float arr[][DTW_SEQUENCE_LEN], int16_t size)
{
	float_t sum = 0;

	for(uint16_t i = 0; i < size; ++i) {
		for(uint16_t j = 0; j < DTW_SEQUENCE_LEN; ++j) {
			sum += arr[i][j];
		}
	}
	return sum;
}


float cityblock(const float x[DTW_SEQUENCE_LEN], rbuf_iterator_t y) {
	float sum = 0;
	for(int i = 0; i < y.size; ++i) {
		sum += fabsf(x[i] - iterate(&y, i));
	}
	return sum;
}


float fastdtw(const float x[DTW_FEATURES][DTW_SEQUENCE_LEN], rbuf_iterator_t y[DTW_FEATURES]) {
	const float pos_inf = 1.0 / 0.0; // srsrly this is correct
	const int16_t size = DTW_FEATURES;

	//	D0 = np.zeros(shape=(size + 1, size + 1))
	volatile float D0[DTW_FEATURES + 1][DTW_FEATURES + 1] = {0};
	//	D1 = D0[1:, 1:]

	for(int16_t i = 1; i <= size; ++i) {
		// D0[0, 1:] = np.inf
		D0[0][i] = pos_inf;
		// D0[1:, 0] = np.inf
		D0[i][0] = pos_inf;
	}

	//	D0[1:, 1:] = cdist(x, y, dist)
	for(int16_t i = 0; i < size; ++i) {
		for(uint16_t j = 0; j < size; ++j) {
			float cbvalue = cityblock(x[i], y[j]);
			D0[i + 1][j + 1] = cbvalue;
		}
	}

	// for i in range(size):
	for(uint16_t i = 0; i < size; ++i) {
		// for j in range(size):
		for(uint16_t j = 0; j < size; ++j) {
			// D1[i, j] += min(D0[i, j], D0[i, j + 1], D0[i + 1, j])
			D0[i + 1][j + 1] += min3(D0[i][j], D0[i][j + 1], D0[i + 1][j]);
		}
	}

	// return D1[-1, -1] / sum(D1.shape)
	float D1_shape_sum = size * 2;
	return D0[size][size] / D1_shape_sum;
}


//fja oblcizajaca koszt dopasowania odpowiednik ffastdtw
float distance(const float x1[DTW_FEATURES][DTW_SEQUENCE_LEN], rbuf_iterator_t y[DTW_FEATURES])
{
    return fastdtw(x1, y); // TODO
}


// koszty nowej probki w porownaniu z istniejacymi (distances w predict_cost)
float costs[BATCH_SIZE] = {0};

//zapamietane probki (self.X)
const float stored_x[BATCH_SIZE][DTW_FEATURES][DTW_SEQUENCE_LEN] = {0};

//zapamietane etykiety problek (self.y)
int stored_y[BATCH_SIZE] = {2,1,1,2,2,4,3,4,4,2,2};


// implementacja https://docs.scipy.org/doc/numpy/reference/generated/numpy.argsort.html
// jako ze na potrzeba K indkesow najmniejszych elkementow i tylko K wiec zatrzymuje sie wczesniej
// array to tablica z kosztami
// indices to tablica w ktorej zapisze znalezione indeksy (rosnaco)
// to jest odpowiednik
// indices = np.argsort(array)
// indices = indices[:K]
void part_argsort(float array[BATCH_SIZE], int indices[K])
{
    int used[BATCH_SIZE] = {0};

    for(int i = 0; i < K; i++)
    {
        int idx = -1;
        float val = +100000;

//        for(int j = idx; j < BATCH_SIZE; j++) // TODO sprawdzić
		for(int j = 0; j < BATCH_SIZE; j++)
        {
            if(array[j] < val && used[j] == 0)
            {
                val = array[j];
                idx = j;
            }
        }
        indices[i] = idx;
        used[idx] = 1;
    }
}

//wybiera najdzesiej pojawiajac sie element w tablicy (njr gestu)
// idices zawiera ktore elementy sprawdzc (jezeli K=3 to beda wybrane tylko te 3 elementy
// z tablcy stored_y i wybrana najczesciej pojawiajaca sie tam wartosc)
// odpowiada
// labels = self.y[neighbours]
// (values, counts) = np.unique(labels, return_counts=True)
// ind = np.argmax(counts)
// pred = values[ind]  # prints the most frequent element
int get_most_frequent_in_array(int indices[K])
{
    int buckets[NUMBER_OF_GESTURES] = {0};

    for(int i = 0; i < K; i++)
    {
    	int ind_i = indices[i];
        int bucket_id = stored_y[ind_i];
        buckets[bucket_id] = buckets[bucket_id] + 1;
    }

    int most_frequent = -1;
    int occurences = 0;
    for(int i = 0; i < NUMBER_OF_GESTURES; i++)
    {
        if(buckets[i] > occurences)
        {
            most_frequent = i;
            occurences = buckets[i];
        }
    }

    return most_frequent;
}


int16_t run_dtw_classifier(rbuf_iterator_t X[DTW_FEATURES])
{
	//oblicz odlegosc do kazdego zapamietanego elementu
    for(int idx = 0; idx < BATCH_SIZE; idx++) {
        costs[idx] = distance(stored_x[idx], X);
    }
    //            0    1  2 3  4 5 6 7  8  9 10
    // to symuluje jakies wyniki
    float costs[BATCH_SIZE] = {1,13,4,5,1,0,8,1,2,5,8};
    		  //BATCH_SIZE] = {2, 1,1,2,2,4,3,4,4,2,2};
    int indices[K]  = {0};
    part_argsort(costs,  indices);

    //zsumuj koszty K najblizszych elementow
    // odpowiada:
    // cost = np.sum(distances[neighbours])
    float sum_of_cost = 0.0f;

    for(int i = 0; i < K; i++) {
        sum_of_cost += costs[indices[i]];
    }
    //sprawdz czy kost dopasowania nie jest za duzy
    // if cost >= self.threshsold:
    //                 predictions[idx] = 'none'

    if(sum_of_cost > THRESHOLD) {
    	return -1;
    }
     else {
        int gesture = get_most_frequent_in_array(indices);
        return gesture;
    }
}
