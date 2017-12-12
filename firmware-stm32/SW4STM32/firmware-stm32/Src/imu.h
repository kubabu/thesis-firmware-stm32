/*
 * imu_sensor.h
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */

#ifndef IMU_H_
#define IMU_H_

#include <math.h>
#include <settings.h>
#include <tm_stm32_ahrs_imu.h>
#include <tm_stm32_disco.h>
#include <tm_stm32_mpu6050.h>
#include <tm_stm32_usart.h>


typedef enum SENSOR_IRQ_STATE {
	SENSOR_NO_DATA_TO_READ = 0,
	SENSOR_DATA_READY_TO_READ = 1
} SENSOR_IRQ_STATE;


typedef enum SENSOR_FIRST_READ_STATE {
	NO_FIRST_READ = 0,
	FIRST_READ_DONE = 1
} SENSOR_FIRST_READ_STATE;


typedef struct IMU_Sensor_Data {
	TM_MPU6050_t reads;
	TM_MPU6050_Interrupt_t reads_interrupts;
	SENSOR_IRQ_STATE irq_flag_state;
	SENSOR_FIRST_READ_STATE first_read_state;
	TM_MPU6050_Result_t init_result;
} IMU_Sensor;


typedef struct EulerAngles {
	float pitch;
	float roll;
	float yaw;
} EulerAngles;


typedef struct IMU_Results {
	float gx, gy, gz;	// gyroscope [deg/sec]
	float ax, ay, az;	// acceleration [G]
//	float mx, my, mz;	// magnetometer [microTesla] not used now
	EulerAngles raw_angles;			// from accelerometer only
	EulerAngles imu_angles;	// with Madgwick filter
} IMU_Results;


#if (DATASET_UPDATE_FREQUENCY_HZ > READS_UPDATE_FREQUENCY_HZ)
#error "Dataset for classifier cannot be updated faster than data is sampled"
#endif


typedef union IMU_Results_t {
	IMU_Results results;
	float results_buffer[FEATURES_COUNT];
} IMU_Results_t;


void IMU_Sensor_Initialize(IMU_Sensor* imu, USART_TypeDef* USARTx);

//void IMU_Sensor_UpdateInterruptFlag(IMU_Sensor* imu, SENSOR_IRQ_STATE state);
//
//void IMU_Sensor_Read_Interrupts(IMU_Sensor* imu);

void IMU_Sensor_Read(IMU_Sensor* imu);


IMU_Results IMU_AHRS_Update(IMU_Sensor* imu, TM_AHRSIMU_t *ahrs);

void AHRS_PrintSerialIMU_Results(USART_TypeDef* USARTx, IMU_Results result);


#endif /* IMU_H_ */
