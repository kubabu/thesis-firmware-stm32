#include "result_processor.h"
#include "tests.h"


volatile MainMode mode = NN_CLASSIFIER_MODE; // SERIAL_FRONTEND_MODE;

USART_TypeDef* USARTx;
IMU_Sensor serial_imu;
TM_AHRSIMU_t serial_ahrs;

char msgbuf[50] = { '\0' };
const char *modes[] = {"NN_CLASSIFIER_MODE", "KNN_CLASSIFIER_MODE", "SERIAL_FRONTEND_MODE", "UNIT_TESTS_MODE"};


int8_t interval_passed(uint32_t now, uint32_t prev, uint32_t interval);

void result_processor_init(USART_TypeDef* serial_port) {
	USARTx = serial_port;
	// TODO params order from example or header?
   TM_AHRSIMU_Init(&serial_ahrs, READS_UPDATE_FREQUENCY_HZ, 0.1f, 3.5f);
}


void process_serial(uint32_t now, classifiers_dataset_t *dataset)
{
	static uint32_t previous_reads_update;
	IMU_Results_t angles;

	IMU_Sensor_Read(&serial_imu);
	if (interval_passed(now, previous_reads_update, READS_UPDATE_INTERVAL_MS)
		  && serial_imu.first_read_state == FIRST_READ_DONE)
	{
		previous_reads_update = now;
		angles.results = IMU_AHRS_Update(&serial_imu, &serial_ahrs);

		if(USARTx != NULL) {
			AHRS_PrintSerialIMU_Results(USARTx, angles.results);
		}
	}
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


void process_reads(uint32_t now, classifiers_dataset_t *dataset) {
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


void check_mode_switch(void) {
	char c = TM_USART_Getc(USART6);
	MainMode m = find_mode(c);
	if(m != mode) {
	  TM_USART_Puts(USART6, "Switching mode to ");
	  TM_USART_Puts(USART6, (char *)modes[m]);
	  TM_USART_Puts(USART6, "\r\n");
	  mode = m;
	}
}


int8_t interval_passed(uint32_t now, uint32_t prev, uint32_t interval) {
	return now >= prev + interval;
}
