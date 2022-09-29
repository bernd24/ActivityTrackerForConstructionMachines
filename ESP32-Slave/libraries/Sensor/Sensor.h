#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

enum SENSOR_NAME {
	MPU6050, BNO055
};



// Class for handling sensors that are connected.
class Sensor {
public:
	static bool initSensors(bool rssi = false);

	static uint8_t getData(float* handle);


	static uint8_t getMPUSensorCount();
	static uint8_t getBNOSensorCount();
	static uint8_t getLIDARSensorCount();
	static uint8_t getSONARSensorCount();
	static bool	   getRSSIFlag();
	

private:
	/*
	TODO: Implement other sensors
	should we have an array of objects or keep them as is?
	*/
	static Adafruit_MPU6050 mpu0;
	static Adafruit_MPU6050 mpu1;
	static Adafruit_BNO055 bno0;

	static bool MPU6050_0_FLAG;
	static bool MPU6050_1_FLAG;

	static bool BNO055_0_FLAG;
	static bool BNO055_1_FLAG;

	static bool LIDAR_0_FLAG;
	static bool LIDAR_1_FLAG;

	static bool SONAR_0_FLAG;
	static bool SONAR_1_FLAG;

	static bool RSSI_FLAG;


};

#endif