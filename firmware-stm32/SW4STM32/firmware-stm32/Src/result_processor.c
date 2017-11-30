#include "result_processor.h"
#include "tests.h"


const char *modes[] = {"NN_CLASSIFIER_MODE", "KNN_CLASSIFIER_MODE", "RAW_READS_MODE", "UNIT_TESTS_MODE"};


void process_reads(uint32_t now, classifiers_dataset_t *dataset) {
	const uint32_t results_update_frequency = 10;	// Hz
	const uint32_t results_update_interval = 1000 / results_update_frequency; // ms
	static uint32_t  previous_results_update = 0;
	const int16_t code_no_result = -1;

	if (dataset->is_ready
			  && interval_passed(now, previous_results_update, results_update_interval)) {
		previous_results_update = now;

		char msgbuf[50] = { '\0' };
		int16_t result_code;
		static int16_t prev_result;

		switch(mode) {
		case NN_CLASSIFIER_MODE:
			result_code = nn_classifier(dataset->series);

			if(result_code != code_no_result && result_code != prev_result) {
				const char *gesture = nn_get_name(result_code);
				sprintf(msgbuf, "NN: [%lu] %s\r\n", now, gesture);
				TM_USART_Puts(USART6, msgbuf);
				prev_result = result_code;
			}
			break;

		case KNN_CLASSIFIER_MODE:
			result_code = knn_classifier(dataset->series);
			if(result_code != code_no_result && result_code != prev_result) {
				const char *gesture = knn_get_name(result_code);
				sprintf(msgbuf, "KNN: [%lu] %s\r\n", now, gesture);
				TM_USART_Puts(USART6, msgbuf);
				prev_result = result_code;
			}
			break;

		case UNIT_TESTS_MODE:
			run_all_tests(USART6);
			break;

		default:
			break;
		}
	}
}

MainMode find_mode(char msg) {
	switch(msg) {
	case 's':
	case 'S':
	case 'r':
	case 'R':
		return RAW_READS_MODE;
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
