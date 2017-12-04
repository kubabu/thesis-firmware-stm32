#ifndef _RESULT_PROCESSOR
#define _RESULT_PROCESSOR

#include "defines.h"
#include "classifiers_dataset.h"


typedef enum MainMode {
	KNN_CLASSIFIER_MODE=0, NN_CLASSIFIER_MODE, RAW_READS_MODE, UNIT_TESTS_MODE
} MainMode;

volatile MainMode mode;


#define results_update_frequency_hz 10;	// Hz


void process_reads(uint32_t now, classifiers_dataset_t *dataset);

MainMode find_mode(char msg);

void check_mode_switch(void);

int8_t interval_passed(uint32_t now, uint32_t prev, uint32_t interval);


#endif // _RESULT_PROCESSOR
