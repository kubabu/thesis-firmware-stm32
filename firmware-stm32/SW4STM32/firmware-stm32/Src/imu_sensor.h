/*
 * imu_sensor.h
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */

#ifndef IMU_SENSOR_H_
#define IMU_SENSOR_H_

#include <math.h>
#include <tm_stm32_ahrs_imu.h>
#include <tm_stm32_disco.h>
#include <tm_stm32_mpu6050.h>
#include <tm_stm32_usart.h>


typedef enum SENSOR_IRQ_STATE {
	SENSOR_NO_DATA,
	SENSOR_DATA_READY
} SENSOR_IRQ_STATE;


typedef struct IMU_Sensor_Data {
	TM_MPU6050_t reads;
	TM_MPU6050_Interrupt_t reads_interrupts;
	SENSOR_IRQ_STATE state_flag;
	TM_MPU6050_Result_t init_result;
	USART_TypeDef* USART;
	TM_AHRSIMU_t ahrs;
} IMU_Sensor;


typedef struct EulerAngles {
	float pitch;
	float roll;
	float yaw;
} EulerAngles;


typedef struct IMU_Results {
	float gx, gy, gz;	// gyroscope [deg/sec]
	float ax, ay, az;	// acceleration [G]
	float mx, my, mz;	// magnetometer [microTesla]
	EulerAngles raw_angles;			// from accelerometer only
	EulerAngles filtered_angles;	// with Madgwick filter
} IMU_Results;

#define FEATURES_COUNT 12

typedef union IMU_Results_t {
	IMU_Results results;
	float results_buffer[FEATURES_COUNT];
} IMU_Results_t;


void IMU_Sensor_Initialize(IMU_Sensor* imu, USART_TypeDef* USARTx);

void IMU_Sensor_UpdateInterruptFlag(IMU_Sensor* imu, SENSOR_IRQ_STATE state);

void IMU_Sensor_Read_Update(IMU_Sensor* imu);

IMU_Results IMU_AHRS_Update(IMU_Sensor* imu);

void AHRS_PrintSerialIMU_Results(USART_TypeDef* USARTx, IMU_Results result);


#endif /* IMU_SENSOR_H_ */
