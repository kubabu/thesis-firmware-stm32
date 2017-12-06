/*
 * imu_tests.c
 *
 *  Created on: Dec 5, 2017
 *      Author: kuba
 */
#include <imu.h>
#include "tests.h"

//
//void PrintSerialIMU_Tests(void) {
//
//}


void benchmark_printserialimu_runtime(int32_t baud) {
	int32_t start, duration;
	const uint32_t expected = 100;
	IMU_Results result;

	TM_USART_Init(USART6, TM_USART_PinsPack_1, baud);

	start = HAL_GetTick();
	AHRS_PrintSerialIMU_Results(USART6, result);
	duration = HAL_GetTick() - start;

	char msg[128];
	sprintf(msg, "Benchmarking PrintSerialIMU on %ld baud %ld [ms]\r\n", baud, duration);
	TM_USART_Puts(USART6, msg);
	check_value(duration <= expected, duration, expected, __FUNCTION__);
}



void _run_imu_tests(void)
{
	benchmark_printserialimu_runtime(9600);
	benchmark_printserialimu_runtime(19200);
	benchmark_printserialimu_runtime(38400);
	benchmark_printserialimu_runtime(57600);
	benchmark_printserialimu_runtime(115200);
}
