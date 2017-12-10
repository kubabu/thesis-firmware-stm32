/*
 * settings.h
 *
 *  Created on: Dec 8, 2017
 *      Author: kuba
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define COM_PORT_BAUD_RATE 115200

#define READS_UPDATE_FREQUENCY_HZ 100
#define DATASET_UPDATE_FREQUENCY_HZ 25
#define RESULTS_UPDATE_FREQUENCY_HZ 10

// data features count (3xacc + 3xgyro + 3xyawpithroll + 3xyawpithrollkalman)
#define FEATURES 12
// duration of reads window sequence [s]
#define WINDOW_DURATION 2
// length of reads window sequence (2s * 25HZ = 50)
#define SEQ_LEN DATASET_UPDATE_FREQUENCY_HZ*2

#define NO_GESTURE_DETECTED -1

#define CLASSIFIER_UPDATE_INTERVAL 1000 / RESULTS_UPDATE_FREQUENCY_HZ

#if SEQ_LEN != 50
#error "NOPE"
#endif


#define FEATURES_COUNT 12  // 3D gyro raw reads + 3D acc raw reads + 3d raw angles + 3d IMU results = 12

#endif /* SETTINGS_H_ */
