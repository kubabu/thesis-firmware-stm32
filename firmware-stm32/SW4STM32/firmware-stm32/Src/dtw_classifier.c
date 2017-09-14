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
#include "tests.h"

// ile sasiadow brac, to samo co self.k
#define K 3
// powyzej jakiego ksotu dopasowania uznajemy ze nic z tego nie bedzie (self.threshold)
#define THRESHOLD 100
// ile gestow mamy zdefiniowane w sumie
#define NUMBER_OF_GESTURES 13
// ile jest zapamietanych probek. to samo co w metodze fit gdyby napisac X.shape[0]
#define BATCH_SIZE 11

#define DTW_SIZE 2 // sequence_len?
#define DTW_YSIZE DTW_SIZE


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


float sum(float arr[][DTW_SIZE], int16_t size)
{
	float_t sum = 0;

	for(uint16_t i = 0; i < size; ++i) {
		for(uint16_t j = 0; j < size; ++j) {
			sum += arr[i][j];
		}
	}
	return sum;
}


float cityblock(float x[DTW_SIZE], float y[DTW_SIZE], int16_t size) {
	float sum = 0;
	for(int i = 0; i < size; ++i) {
		sum += fabsf(x[i] - y[i]);
	}
	return sum;
}


float fastdtw(float x[][DTW_SIZE], float y[][DTW_SIZE]) {
	//	D0 = np.zeros(shape=(size + 1, size + 1))
	volatile float D0[DTW_SIZE + 1][DTW_YSIZE + 1] = {0};
	//	D1 = D0[1:, 1:]  CHECK
	volatile float D1[DTW_SIZE][DTW_YSIZE] = {0};
	float pos_inf = 1.0 / 0.0; // xD totally correct carry on
	const int16_t size = DTW_SIZE;
	const int16_t ysize = DTW_YSIZE;

	for(int16_t i = 1; i <= size; ++i) {
		// D0[0, 1:] = np.inf
		D0[0][i] = pos_inf;
		// D0[1:, 0] = np.inf
		D0[i][0] = pos_inf;
	}

	//	D0[1:, 1:] = cdist(x, y, dist)
	for(int16_t i = 0; i < size; ++i) {
		for(uint16_t j = 0; j < ysize; ++j) {
			float cbvalue = cityblock(x[i], y[j], size);
			D0[i + 1][j + 1] = cbvalue;
			D1[i][j] = cbvalue;
		}
	}

	// for i in range(size):
	for(uint16_t i = 0; i < size; ++i) {
		// for j in range(size):
		for(uint16_t j = 0; j < ysize; ++j) {
			// D1[i, j] += min(D0[i, j], D0[i, j + 1], D0[i + 1, j])
			D1[i][j] += min3(D0[i][j], D0[i][j + 1], D0[i + 1][j]);
		}
	}

	// return D1[-1, -1] / sum(D1.shape)
	float D1_shape_sum = size * 2;
	float result = D1[size - 1][ysize - 1] / D1_shape_sum;

	return result;
}


uint8_t cityblock_tests(USART_TypeDef *usart) {
	float x[2] = {0.0, 0.0};
	float y[2] = {0.0, 0.0};
	float result = cityblock(x, y, 2);

	if(result != 0.0) {
		TM_USART_Puts(usart, "test1 failed");
	}
	float x2[2] = {1.0, 2.0};
	float y2[2] = {1.0, 3.0};
	result = cityblock(x2, y2, 2);
	if(result != 1.0) {
		TM_USART_Puts(usart, "test2 is failed");
	}
	float x3[2] = {1.0, 2.0};
	float y3[2] = {2.0, 3.0};
	result = cityblock(x3, y3, 2);
	if(result != 2.0) {
		TM_USART_Puts(usart, "test3 failed");
	}
	return 0;
}


uint8_t fastdtw_tests(USART_TypeDef *usart) {
	float x0[2][2] = {{0.0, 0.0},
					{0.0, 0.0}};
	float y0[2][2] = {{0.0, 0.0},
					{0.0, 0.0}};
	volatile float result = fastdtw(x0, y0);
	if(result != 0.0) {
		TM_USART_Puts(usart, "fastdtw test failed");
	}

	float x2[2][2] = {{0.0, 0.0},
					  {0.0, 0.0}};
	float y2[2][2] = {{1.0, 1.0},
					  {1.0, 1.0}};
	result = fastdtw(x2, y2);
	if(result != 1.0) {
		TM_USART_Puts(usart, "fastdtw test2 failed");
	}

	float x3[2][2] = {{1.0, 2.0},
				      {1.0, 2.0}};
	float y3[2][2] = {{2.0, 3.0},
					  {3.0, 4.0}};
	result = fastdtw(x3, y3);
	if(result != 1.5) {
		TM_USART_Puts(usart, "fastdtw test3 failed");
	}

	return 0;
}

uint8_t dtw_tests(USART_TypeDef *usart) {
	cityblock_tests(usart);
	fastdtw_tests(usart);
	return 0;
}
//
//
////fja oblcizajaca koszt dopasowania odpowiednik ffastdtw
//float distance(float x1[FEATURES][SEQUENCE_LEN], float x[FEATURES][SEQUENCE_LEN])
//{
//    return 1.0f;
//}
//
//// koszty nowej probki w porownaniu z istniejacymi (distances w predict_cost)
//float costs[BATCH_SIZE] = {0};
//
////zapamietane probki (self.X)
//float stored_x[BATCH_SIZE][FEATURES][SEQUENCE_LEN] = {0};
//
////zapamietane etykiety problek (self.y)
//int stored_y[BATCH_SIZE] = {2,1,1,2,2,4,3,4,4,2,2};
//
////dane do sprawdzemia parametr X w predict cost
//float X[FEATURES][SEQUENCE_LEN] = {0};
//
//// printuje liste floatow
//// //printuje liste intow
////void iprint_seq(const char *name, int *seq, int len)
////{
////    puts(name);
////    for(int i = 0; i < len; i++)
////    {
////        printf("%d ", seq[i]);
////    }
////
////    putchar('\n');
////}
//
//// implementacja https://docs.scipy.org/doc/numpy/reference/generated/numpy.argsort.html
//// jako ze na potrzeba K indkesow najmniejszych elkementow i tylko K wiec zatrzymuje sie wczesniej
//// array to tablica z kosztami
//// indices to tablica w ktorej zapisze znalezione indeksy (rosnaco)
//// to jest odpowiednik
//// indices = np.argsort(array)
//// indices = indices[:K]
//void part_argsort(float* array, int *indices)
//{
//    int used[BATCH_SIZE] = {0};
//
//    for(int i = 0; i < K; i++)
//    {
//        int idx = -1;
//        float val = +100000;
//
//        for(int j = idx; j < BATCH_SIZE; j++)
//        {
//            if(array[j] < val && used[j] == 0)
//            {
//                val = array[j];
//                idx = j;
//            }
//        }
//        indices[i] = idx;
//        used[idx] = 1;
//    }
//}
//
////wybiera najdzesiej pojawiajac sie element w tablicy (njr gestu)
//// idices zawiera ktore elementy sprawdzc (jezeli K=3 to beda wybrane tylko te 3 elementy
//// z tablcy stored_y i wybrana najczesciej pojawiajaca sie tam wartosc)
//// odpowiada
//// labels = self.y[neighbours]
//// (values, counts) = np.unique(labels, return_counts=True)
//// ind = np.argmax(counts)
//// pred = values[ind]  # prints the most frequent element
//int get_most_frequent_in_array(int *indices)
//{
//    int buckets[NUMBER_OF_GESTURES] = {0};
//
//    for(int i = 0; i < K; i++)
//    {
//        int bucket_id = stored_y[indices[i]];
//        buckets[bucket_id] = buckets[bucket_id] + 1;
//    }
//
//    int most_frequent = -1;
//    int occurences = 0;
//    for(int i = 0; i < NUMBER_OF_GESTURES; i++)
//    {
//        if(buckets[i] > occurences)
//        {
//            most_frequent = i;
//            occurences = buckets[i];
//        }
//    }
//
//    return most_frequent;
//}
//
//
//int16_t run_dtw_classifier()
//{
//    //oblicz odlegosc do kazdego zapamietanego elementu
//    for(int idx = 0; idx < BATCH_SIZE; idx++)
//    {
//        costs[idx] = distance(stored_x[idx], X);
//    }
//    //            0    1  2 3  4 5 6 7  8  9 10
//    // to symuluje jakies wyniki
//    float costs[] = {1,13,4,5,1,0,8,1,2,5,8};
//    //BATCH_SIZE] = {2, 1,1,2,2,4,3,4,4,2,2};
//    int indices[K]  = {0};
//    part_argsort(costs,  indices);
//
//    //zsumuj koszty K najblizszych elementow
//    // odpowiada:
//    // cost = np.sum(distances[neighbours])
//    float sum_of_cost = 0.0f;
//
//    for(int i = 0; i < K; i++)
//    {
//        sum_of_cost += costs[indices[i]];
//    }
//    //sprawdz czy kost dopasowania nie jest za duzy
//    // if cost >= self.threshsold:
//    //                 predictions[idx] = 'none'
//
//    if(sum_of_cost > THRESHOLD)
//    {
//    	return -1;
//    }
//    else
//    {
//        int gesture = get_most_frequent_in_array(indices);
//        return gesture;
//    }
//}
