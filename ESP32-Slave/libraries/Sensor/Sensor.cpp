#include "Sensor.h"


Sensor::Sensor(SENSOR_NAME sensor_type)
: name(sensor_type) {}

bool Sensor::begin() {
	switch (sensor_type) {
		case MPU6050:
			if(mpu.begin())
				return true;
			break;
		case BNO055:
			bno.
			break;
		default:
			~Sensor();
	}
}
