#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

enum SENSOR_NAME {
	MPU6050, BNO055
}

/*
class Sensor {
public:
	explicit Sensor(SENSOR_NAME sensor_type);

	bool begin();

	void getData(float* handle);

	uint8_t getDataCount();
	String getID();

private:
	SENSOR_NAME name;
	uint8_t id;

	Adafruit_MPU6050 mpu;
	Adafruit_BNO055 bno;

	static uint8_t MPU6050_ID_COUNTER = 0;
	static uint8_t BNO055_ID_COUNTER = 0;
}
*/
#endif