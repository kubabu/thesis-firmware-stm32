/*
 * ahrs.c
 *
 *  Created on: Sep 8, 2017
 *      Author: kuba
 */
#include <imu.h>


void AHRS_PrintSerial(USART_TypeDef* USARTx, float x, float y, float z) {
	const int16_t buffer_size = 150;
	char data[buffer_size];
	uint16_t data_size = 0;

	data_size = sprintf(data, "%.2f, %.2f, %.2f", x, y, z); //, AHRSIMU.Roll, AHRSIMU.Pitch, AHRSIMU.Yaw);

	if(data_size) {
		TM_USART_Puts(USARTx, data);
	}
}


void AHRS_PrintSerialEulers(USART_TypeDef* USARTx, EulerAngles angles) {
	AHRS_PrintSerial(USARTx, angles.pitch, angles.roll, angles.yaw);
}


void AHRS_PrintSerialIMU_Results(USART_TypeDef* USARTx, IMU_Results result) {
	AHRS_PrintSerial(USARTx, result.ax, result.ay, result.az);
	TM_USART_Puts(USARTx, ", ");

	AHRS_PrintSerial(USARTx, result.gx, result.gy, result.gz);
	TM_USART_Puts(USARTx, ", ");

	AHRS_PrintSerialEulers(USARTx, result.raw_angles);
	TM_USART_Puts(USARTx, ", ");

	AHRS_PrintSerialEulers(USARTx, result.imu_angles);
	TM_USART_Puts(USARTx, "\r\n");
}


float rotate180(float angle) {
	if (angle > 0.0f) {
		return angle - 180.0f;
	} else {
		return angle + 180.0f;
	}
}


EulerAngles CalculateFromCalibratedAccelerometer(float ax, float ay, float az) {
    EulerAngles result;

	result.pitch = 180 * atan(ax / sqrt(ay * ay + az * az)) / M_PI;
	result.roll = 180 * atan (ay / sqrt(ax * ax + az * az)) / M_PI;
	result.yaw = 180 * atan (az / sqrt(ax * ax + az * az)) / M_PI;

	return result;
}


IMU_Results IMU_AHRS_Update(IMU_Sensor* imu) {
	IMU_Results result;

    const float dpsRangePerDigit = imu->reads.Gyro_Mult;
    const float accRangePerDigit = imu->reads.Acce_Mult;

    /* Convert data to gees, deg/sec and microTesla respectively */
    result.gx = imu->reads.Gyroscope_X * dpsRangePerDigit;
    result.gy = imu->reads.Gyroscope_Y * dpsRangePerDigit;
    result.gz = imu->reads.Gyroscope_Z * dpsRangePerDigit;

    result.ax = imu->reads.Accelerometer_X * accRangePerDigit;
    result.ay = imu->reads.Accelerometer_Y * accRangePerDigit;
    result.az = imu->reads.Accelerometer_Z * accRangePerDigit;

//    result.mx = result.my = result.mz = 0; // no magnetometer on MPU6050

    result.raw_angles = CalculateFromCalibratedAccelerometer(result.ax, result.ay, result.az); // will it blend?
    /* Call update function */
    /* This function must be called periodically in intervals set by sample rate on initialization process */
//    TM_AHRSIMU_UpdateAHRS(&imu->ahrs, AHRSIMU_DEG2RAD(result.gx), AHRSIMU_DEG2RAD(result.gy), AHRSIMU_DEG2RAD(result.gz),
//    		result.ax, result.ay, result.az, result.mx, result.my, result.mz);
    TM_AHRSIMU_UpdateIMU(&imu->ahrs, AHRSIMU_DEG2RAD(result.gx), AHRSIMU_DEG2RAD(result.gy), AHRSIMU_DEG2RAD(result.gz),
        		result.ax, result.ay, result.az);

    result.imu_angles.pitch = imu->ahrs.Pitch;
    result.imu_angles.roll = rotate180(imu->ahrs.Roll); //for some reason roll is backwards on STM
    result.imu_angles.yaw = imu->ahrs.Yaw;

    return result;
}

