#include "Sensors.h"
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 Sensors::mpu0;
Adafruit_MPU6050 Sensors::mpu1;

HC_SR04 Sensors::sonar_0;
TFMPI2C Sensors::lidar_0;

bool Sensors::MPU6050_0_FLAG = false;
bool Sensors::MPU6050_1_FLAG = false;
bool Sensors::LIDAR_0_FLAG 	 = false;
bool Sensors::SONAR_0_FLAG 	 = false;
bool Sensors::RSSI_FLAG 	 = false;

void Sensors::setAllFlags(bool b) {
	MPU6050_0_FLAG = b;
	MPU6050_1_FLAG = b;
	LIDAR_0_FLAG = b;
	SONAR_0_FLAG = b;
	RSSI_FLAG = b;
}

bool Sensors::init(bool rssi) {
	// Try every sensor object to see what is connected.
	setAllFlags(false);
	bool is_any_flag_set = false;
	if(mpu0.begin()) {
		Serial.println("Found a mpu");
		MPU6050_0_FLAG = true;
		is_any_flag_set = true;
	}

	if(mpu1.begin(0x69)) {
		Serial.println("Found second mpu");
		MPU6050_1_FLAG = true;
		is_any_flag_set = true;
	}

	// Test if HC_SR04 is connected
	pinMode(HC_SR04_TRIG_PIN, OUTPUT);
	pinMode(HC_SR04_ECHO_PIN, INPUT);

	if(sonar_0.getDistance() != 0.0) {
		SONAR_0_FLAG = true;
		is_any_flag_set = true;
	}

	if(lidar_0.sendCommand(SOFT_RESET, 0)) {
		LIDAR_0_FLAG = true;
		is_any_flag_set = true;
	}


	RSSI_FLAG = rssi;
	if(rssi) is_any_flag_set = true;

	return is_any_flag_set;

	/*
	TODO: Implement other sensors ?
	*/
}

uint8_t Sensors::getData(float* handle) {
	uint8_t float_count = 0;
	sensors_event_t a, b, c;

	if(MPU6050_0_FLAG) {
		mpu0.getEvent(&a, &b, &c);
		handle[float_count++] = a.acceleration.x;
		handle[float_count++] = a.acceleration.y;
		handle[float_count++] = a.acceleration.z;

		handle[float_count++] = b.acceleration.x;
		handle[float_count++] = b.acceleration.y;
		handle[float_count++] = b.acceleration.z;
	}

	if(MPU6050_1_FLAG) {
		mpu1.getEvent(&a, &b, &c);
		handle[float_count++] = a.acceleration.x;
		handle[float_count++] = a.acceleration.y;
		handle[float_count++] = a.acceleration.z;

		handle[float_count++] = b.acceleration.x;
		handle[float_count++] = b.acceleration.y;
		handle[float_count++] = b.acceleration.z;
	}


	if(SONAR_0_FLAG) {
		handle[float_count++] = sonar_0.getDistance();
	}

	if(LIDAR_0_FLAG) {
		int16_t dist = 0;
		lidar_0.getData(dist);
		handle[float_count++] = (float)dist;
	}

	return float_count;
}

uint8_t Sensors::getMPUSensorCount() {
	uint8_t count = 0;

	if(MPU6050_0_FLAG) ++count;
	if(MPU6050_1_FLAG) ++count;

	return count;
}

uint8_t Sensors::getLIDARSensorCount() {
	uint8_t count = 0;

	if(LIDAR_0_FLAG) ++count;

	return count;
}

uint8_t Sensors::getSONARSensorCount() {
	uint8_t count = 0;

	if(SONAR_0_FLAG) ++count;

	return count;
}

bool Sensors::getRSSIFlag() {
	return RSSI_FLAG;
}