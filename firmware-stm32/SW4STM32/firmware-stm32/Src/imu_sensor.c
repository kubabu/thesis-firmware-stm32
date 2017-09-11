/*
 * imu_sensor.c
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */
#include "imu_sensor.h"


void IMU_Sensor_Initialize(IMU_Sensor* imu, USART_TypeDef* USARTx) {
	TM_MPU6050_t* MPU6050 = &(imu->reads);
	imu->state_flag = SENSOR_NO_DATA;
	imu->USART = USARTx;
	imu->init_result =  TM_MPU6050_Init(&imu->reads, TM_MPU6050_Device_0,
				  TM_MPU6050_Accelerometer_2G, TM_MPU6050_Gyroscope_2000s);

	if (imu->init_result == TM_MPU6050_Result_Ok) {
		TM_DISCO_LedOn(LED_GREEN);
	} else {
	  while(1){
		  if(USARTx != NULL) {
			  TM_USART_Puts(USARTx, "MPU6050 not initialized properly, will not start");
		  }

		  TM_DISCO_LedToggle(LED_RED);
		  HAL_Delay(500);
	  }
	}

	TM_MPU6050_SetDataRate(MPU6050, TM_MPU6050_DataRate_100Hz);
	TM_MPU6050_EnableInterrupts(MPU6050);

	/* Init structure with 100hZ sample rate, 0.1 beta and 3.5 inclination
	* (3.5 degrees is inclination in Ljubljana, Slovenia) on July, 2016 */
	TM_AHRSIMU_Init(&(imu->ahrs), 100, 0.1f, 3.5f);
}


void IMU_Sensor_UpdateInterruptFlag(IMU_Sensor* imu, SENSOR_IRQ_STATE state) {
	imu->state_flag = state;
}


uint8_t IMU_IsReadyToRead(IMU_Sensor* imu) {
	return imu->init_result == TM_MPU6050_Result_Ok
			&& imu->state_flag == SENSOR_DATA_READY;
}


void IMU_Sensor_Read_Update(IMU_Sensor* imu) {
	if(IMU_IsReadyToRead(imu)) {
		TM_MPU6050_ReadInterrupts(&imu->reads, &imu->reads_interrupts); // check sensor

		if (imu->reads_interrupts.F.DataReady) {
			TM_MPU6050_ReadAll(&imu->reads);
		}
		imu->state_flag = SENSOR_NO_DATA;
	}
}