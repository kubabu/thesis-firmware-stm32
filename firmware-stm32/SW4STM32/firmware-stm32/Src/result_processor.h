#ifndef _RESULT_PROCESSOR
#define _RESULT_PROCESSOR

#include "defines.h"
#include "classifiers_dataset.h"
#include "settings.h"


typedef enum MainMode {
	KNN_CLASSIFIER_MODE, NN_CLASSIFIER_MODE, SERIAL_FRONTEND_MODE, UNIT_TESTS_MODE
} MainMode;

void result_processor_init(USART_TypeDef* serial_port);

void process_reads(uint32_t now, classifiers_dataset_t *dataset);

void check_mode_switch(void);

#endif // _RESULT_PROCESSOR
