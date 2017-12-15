#include "main.h"
#include "result_processor.h"
#include "tests.h"


volatile MainMode mode =  KNN_CLASSIFIER_MODE;

USART_TypeDef* USARTx;
//IMU_Sensor serial_imu;
//TM_AHRSIMU_t serial_ahrs;

char msgbuf[50] = { '\0' };
const char *modes[] = {"NN_CLASSIFIER_MODE", "KNN_CLASSIFIER_MODE", "SERIAL_FRONTEND_MODE", "UNIT_TESTS_MODE"};


int8_t interval_passed(uint32_t now, uint32_t prev, uint32_t interval);

void Result_process_Initialize(USART_TypeDef* serial_port) {
	USARTx = serial_port;
//	// TODO params order + do I need separate IMU for 100hz?
//   TM_AHRSIMU_Init(&serial_ahrs, SERIAL_READS_UPDATE_FREQUENCY_HZ, 0.1f, 3.5f);

	if(mode == SERIAL_FRONTEND_MODE) {
		// serial reads to PC needs to be in 100Hz
		Dataset_Set_Update_Frequency(SERIAL_READS_UPDATE_INTERVAL_MS);
	} else {
		// dataset for recognition is updated at 25Hz
		Dataset_Set_Update_Frequency(DATASET_UPDATE_INTERVAL_MS);
	}
}


void process_serial(uint32_t now, classifiers_dataset_t *dataset)
{
//	static uint32_t previous_reads_update;
//	if (interval_passed(now, previous_reads_update, SERIAL_READS_UPDATE_INTERVAL_MS)
//			&& dataset->queue_size)
//	{
//
//	}

	// dummy process method, for right timing all is done in sensor update
	// do not use dataset series but last enqueued element
	// so do not process (push on series) queued results at all
}


void process_knn(uint32_t now, classifiers_dataset_t *dataset)
{
	static uint32_t  previous_results_update = 0;
//	const uint32_t results_display_break = 2000;
//	static uint32_t  previous_results_display = 0;
//	static int16_t prev_result;

	if (dataset->is_ready
			&& interval_passed(now, previous_results_update, CLASSIFIER_UPDATE_INTERVAL_MS))
	{
		previous_results_update = now;

		int16_t result_code = knn_classifier(dataset->series);

		// TODO remove after result serial transmit test
		result_code = 2;

		if(result_code != NO_GESTURE_DETECTED
//			&& result_code != prev_result
//			&& now >= previous_results_display + results_display_break
		) {
			const char *gesture = knn_get_name(result_code);
			sprintf(msgbuf, "KNN: [%lu] %s\r\n", now, gesture);
			TM_USART_Puts(USARTx, msgbuf);
//			prev_result = result_code;
//			previous_results_display = now;
		}
	}
}


void process_nn(uint32_t now, classifiers_dataset_t *dataset)
{
	static uint32_t  previous_results_update = 0;
//	static int16_t prev_result;

	if (dataset->is_ready
			&& interval_passed(now, previous_results_update, CLASSIFIER_UPDATE_INTERVAL_MS)) {
		previous_results_update = now;

		int16_t result_code = nn_classifier(dataset->series);

		if(result_code != NO_GESTURE_DETECTED
		) {
			const char *gesture = knn_get_name(result_code);
			sprintf(msgbuf, "KNN: [%lu] %s\r\n", now, gesture);
			TM_USART_Puts(USARTx, msgbuf);
//			prev_result = result_code;
		}
	}
}


void Result_process_Reads(uint32_t now, classifiers_dataset_t *dataset) {
	switch(mode) {
	case NN_CLASSIFIER_MODE:
		process_nn(now, dataset);
		break;

	case KNN_CLASSIFIER_MODE:
		process_knn(now, dataset);
		break;

	case UNIT_TESTS_MODE:
		run_all_tests(USARTx);
		break;

	case SERIAL_FRONTEND_MODE:
		process_serial(now, dataset);
		break;

	default:
		break;
	}
}


MainMode find_mode(char msg) {
	switch(msg) {
	case 's':
	case 'S':
	case 'r':
	case 'R':
	case 'f':
	case 'F':
		return SERIAL_FRONTEND_MODE;
		break;

	case 'n':
	case 'N':
		return NN_CLASSIFIER_MODE;
		break;

	case 'k':
	case 'K':
	case 'd':
	case 'D':
		return KNN_CLASSIFIER_MODE;
		break;

	case 't':
	case 'T':
	case 'u':
	case 'U':
		return UNIT_TESTS_MODE;
		break;

	default:
		return mode;
		break;
	}
}



void Result_process_Check_Mode(void) {
	char c = TM_USART_Getc(USART6);
	MainMode m = find_mode(c);
	if(m != mode) {
	  TM_USART_Puts(USART6, "Switching mode to ");
	  TM_USART_Puts(USART6, (char *)modes[m]);
	  TM_USART_Puts(USART6, "\r\n");

	  if(m == SERIAL_FRONTEND_MODE) {
		  // serial reads to PC needs to be in 100Hz
		  Dataset_Set_Update_Frequency(SERIAL_READS_UPDATE_INTERVAL_MS);
	  } else if(mode == SERIAL_FRONTEND_MODE) {
		  // dataset update has reduced frequency
		  Dataset_Set_Update_Frequency(DATASET_UPDATE_INTERVAL_MS);
	  }
	  mode = m;
	}
}


int8_t interval_passed(uint32_t now, uint32_t prev, uint32_t interval) {
	return now >= prev + interval;
}
