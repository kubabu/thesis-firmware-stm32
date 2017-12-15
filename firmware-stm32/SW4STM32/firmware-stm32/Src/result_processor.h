#ifndef _RESULT_PROCESSOR
#define _RESULT_PROCESSOR

#include "defines.h"
#include "classifiers_dataset.h"
#include "settings.h"


typedef enum MainMode {
	KNN_CLASSIFIER_MODE, NN_CLASSIFIER_MODE, SERIAL_FRONTEND_MODE, UNIT_TESTS_MODE
} MainMode;


void Result_process_Initialize(USART_TypeDef* serial_port);

void Result_process_Reads(uint32_t now, classifiers_dataset_t *dataset);

void Result_process_Check_Mode(void);

#endif // _RESULT_PROCESSOR
