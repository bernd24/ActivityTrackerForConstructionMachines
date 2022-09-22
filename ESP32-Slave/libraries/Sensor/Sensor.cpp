#include "Sensor.h"

Sensor::Sensor(SENSOR_NAME sensor_type)
: name(sensor_type)
{
	switch (sensor_type) {
		case MPU6050:
			id = MPU6050_ID_COUNTER++;
			break;
		case BNO055:
			id = BNO055_ID_COUNTER++;
			break;
		default:
			~Sensor();
	}
}