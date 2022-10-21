#include "Sensors.h"
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 Sensors::mpu0;
Adafruit_MPU6050 Sensors::mpu1;

HC_SR04 Sensors::sonar_0;
SoftwareSerial Sensors::lidar_serial(lidar_rx_pin, lidar_tx_pin);

bool Sensors::MPU6050_0_FLAG = false;
bool Sensors::MPU6050_1_FLAG = false;
bool Sensors::LIDAR_0_FLAG 	 = false;
bool Sensors::SONAR_0_FLAG 	 = false;
bool Sensors::RSSI_FLAG 	 = false;

uint64_t lidar_timer;
const uint64_t lidar_init_timeout = 2000;
const uint64_t lidar_measure_timeout = 100;

void Sensors::setAllFlags(bool b) {
	MPU6050_0_FLAG = b;
	MPU6050_1_FLAG = b;
	LIDAR_0_FLAG = b;
	SONAR_0_FLAG = b;
	RSSI_FLAG = b;
}

void Sensors::getTFminiDistance(int* dist, int* str) {
	static char i = 0;
	char j = 0;
	int checksum = 0;
	static int rx[9];
	if(!lidar_serial.available()) {
		return;
	}
	
	rx[i] = lidar_serial.read();
	if (rx[0] != 0x59) {
      i = 0;
    }
    else if (i == 1 && rx[1] != 0x59) {
      i = 0;
    }
    else if (i == 8)
    {
      for (j = 0; j < 8; j++)
      {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256))
      {
        *dist = rx[2] + rx[3] * 256;
        *str = rx[4] + rx[5] * 256;
      }
      i = 0;
    }
    else
    {
      i++;
    }
}
bool Sensors::init(bool rssi) {
	// Try every sensor object to see what is connected.
	//lidar_serial(lidar_rx_pin, lidar_tx_pin);
	//lidar_serail = SoftwareSerial()
	lidar_serial.begin(115200);
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

	int dist = 0;
	int str = 0;
	lidar_timer = millis();
	while(!dist) {
		getTFminiDistance(&dist, &str);
		if(dist) {
			LIDAR_0_FLAG = true;
			is_any_flag_set = true;
			break;
		}
		if(millis() - lidar_timer > lidar_init_timeout) {
			Serial.println("Timeout reached for lidar");
			break;
		}
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

		handle[float_count++] = b.gyro.x;
		handle[float_count++] = b.gyro.y;
		handle[float_count++] = b.gyro.z;
	}

	if(MPU6050_1_FLAG) {
		mpu1.getEvent(&a, &b, &c);
		handle[float_count++] = a.acceleration.x;
		handle[float_count++] = a.acceleration.y;
		handle[float_count++] = a.acceleration.z;

		handle[float_count++] = b.gyro.x;
		handle[float_count++] = b.gyro.y;
		handle[float_count++] = b.gyro.z;
	}


	if(SONAR_0_FLAG) {
		handle[float_count++] = sonar_0.getDistance();
	}

	if(LIDAR_0_FLAG) {
		int dist = 0;
		int str = 0;
		lidar_timer = millis();
		while(!dist) {
			getTFminiDistance(&dist, &str);
			if(dist) {
				break;
			}
			if(millis() - lidar_timer > lidar_measure_timeout) {
				Serial.println("Timeout reached for lidar");
				break;
			}
		}
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