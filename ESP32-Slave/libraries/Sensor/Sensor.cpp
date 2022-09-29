#include "Sensor.h"


bool Sensor::initSensors(bool rssi) {
	// Try every sensor object to see what is connected.
	if(mpu0.begin()) {
		MPU6050_0_FLAG = true;
	}

	if(mpu1.begin()) {
		MPU6050_1_FLAG = true;
	}

	if(bno0.begin()) {
		BNO055_0_FLAG = true;
	}

	RSSI_FLAG = rssi;

	/*
	TODO: Implement other sensors
	*/
}

uint8_t Sensor::getData(float* handle) {
	uint8_t float_count = 0;
	/*
	if(MPU6050_0_FLAG) {
		mpu0.
	}
	*/
}

uint8_t Sensor::getMPUSensorCount() {
	uint8_t count = 0;

	if(MPU6050_0_FLAG) ++count;
	if(MPU6050_1_FLAG) ++count;

	return count;
}

uint8_t Sensor::getBNOSensorCount() {
	uint8_t count = 0;

	if(BNO055_0_FLAG) ++count;
	if(BNO055_1_FLAG) ++count;

	return count;
}

uint8_t Sensor::getLIDARSensorCount() {
	uint8_t count = 0;

	if(LIDAR_0_FLAG) ++count;
	if(LIDAR_1_FLAG) ++count;

	return count;
}

uint8_t Sensor::getSONARSensorCount() {
	uint8_t count = 0;

	if(SONAR_0_FLAG) ++count;
	if(SONAR_1_FLAG) ++count;

	return count;
}

bool Sensor::getRSSIFlag() {
	return RSSI_FLAG;
}