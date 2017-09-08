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
	imu->init_result =  TM_MPU6050_Init(MPU6050, TM_MPU6050_Device_0,
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
	if(imu->init_result == TM_MPU6050_Result_Ok && imu->state_flag == SENSOR_DATA_READY) {
		return 1;
	}
	return 0;
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

void AHRS_PrintSerial(USART_TypeDef* USARTx, float x, float y, float z) {
	const int16_t buffer_size = 150;
	char data[buffer_size];
	uint16_t data_size = 0;

	data_size = sprintf(data, "%.2f, %.2f, %.2f", x, y, z); //, AHRSIMU.Roll, AHRSIMU.Pitch, AHRSIMU.Yaw);

	if(data_size) {
		TM_USART_Puts(USARTx, data);
	}
}

float rotate180(float angle) {
	if (angle > 0.0f) {
		return angle - 180.0f;
	} else {
		return angle + 180.0f;
	}
}


EulerAngles CalculateFromCalibratedAccelerometer(float ax, float ay, float az)
{
    EulerAngles result;

	result.pitch = 180 * atan(ax / sqrt(ay * ay + az * az)) / M_PI;
	result.roll = 180 * atan (ay / sqrt(ax * ax + az * az)) / M_PI;
	result.yaw = 180 * atan (az / sqrt(ax * ax + az * az)) / M_PI;

	return result;
}

EulerAngles IMU_AHRS_Update(IMU_Sensor* imu, IMU_Results* results) {
//    float gx, gy, gz, ax, ay, az, mx, my, mz;
    EulerAngles result;

    const float dpsRangePerDigit = imu->reads.Gyro_Mult;
    const float accRangePerDigit = imu->reads.Acce_Mult;

    /* Convert data to gees, deg/sec and microTesla respectively */
    results->gx = imu->reads.Gyroscope_X * dpsRangePerDigit;
    results->gy = imu->reads.Gyroscope_Y * dpsRangePerDigit;
    results->gz = imu->reads.Gyroscope_Z * dpsRangePerDigit;

    results->ax = imu->reads.Accelerometer_X * accRangePerDigit;
    results->ay = imu->reads.Accelerometer_Y * accRangePerDigit;
    results->az = imu->reads.Accelerometer_Z * accRangePerDigit;

    results->mx = results->my = results->mz = 0; // no magnetometer on MPU6050

    result = CalculateFromCalibratedAccelerometer(results->ax, results->ay, results->az);
    /* Call update function */
    /* This function must be called periodically in intervals set by sample rate on initialization process */
    TM_AHRSIMU_UpdateAHRS(&imu->ahrs, AHRSIMU_DEG2RAD(results->gx), AHRSIMU_DEG2RAD(results->gy), AHRSIMU_DEG2RAD(results->gz),
    		results->ax, results->ay, results->az, results->mx, results->my, results->mz);

    if(imu->USART != NULL)
    {
		AHRS_PrintSerial(imu->USART, results->ax, results->ay, results->az);
		TM_USART_Puts(imu->USART, ", ");

		AHRS_PrintSerial(imu->USART, results->gx, results->gy, results->gz);
		TM_USART_Puts(imu->USART, ", ");

		AHRS_PrintSerial(imu->USART, results->angles.pitch, results->angles.roll, results->angles.yaw);
		TM_USART_Puts(imu->USART, ", ");

		// roll of AHRSIMU
		AHRS_PrintSerial(imu->USART, imu->ahrs.Pitch, rotate180(imu->ahrs.Roll), imu->ahrs.Yaw);
		TM_USART_Puts(imu->USART, "\r\n");
    }

    return result;
}
