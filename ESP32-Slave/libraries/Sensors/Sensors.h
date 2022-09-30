#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TFMPI2C.h>

#define HC_SR04_TRIG_PIN 12
#define HC_SR04_ECHO_PIN 13

#define SOUND_SPEED 0.034

struct HC_SR04 {
	uint8_t trigger_pin = HC_SR04_TRIG_PIN;
	uint8_t echo_pin = HC_SR04_ECHO_PIN;

	float getDistance() {
		digitalWrite(HC_SR04_TRIG_PIN, LOW);
		delayMicroseconds(2);
		digitalWrite(HC_SR04_TRIG_PIN, HIGH);
		delayMicroseconds(10);
		digitalWrite(HC_SR04_TRIG_PIN, LOW);

		unsigned long duration = pulseIn(HC_SR04_ECHO_PIN, HIGH);
		float distance = (float)duration*SOUND_SPEED / 2;

		return distance;
	}
};

// Class for handling sensors that are connected.
class Sensors {
public:
	static bool init(bool rssi = false);

	static uint8_t getData(float* handle);


	static uint8_t getMPUSensorCount();
	static uint8_t getLIDARSensorCount();
	static uint8_t getSONARSensorCount();
	static bool	   getRSSIFlag();
	

private:
	/*
	TODO: Implement other sensors
	should we have an array of objects or keep them as is?
	Difficult to add support for new sensors this way.
	*/

	static void setAllFlags(bool b);


	static Adafruit_MPU6050 mpu0;
	static Adafruit_MPU6050 mpu1;

	static HC_SR04 sonar_0;

	static TFMPI2C lidar_0;

	static bool MPU6050_0_FLAG;
	static bool MPU6050_1_FLAG;

	static bool LIDAR_0_FLAG;

	static bool SONAR_0_FLAG;

	static bool RSSI_FLAG;


};

#endif