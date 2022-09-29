#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()

// Global copy of slave
static esp_now_peer_info_t slave;
#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0

// GLOBAL CONSTANTS
const uint8_t HANDSHAKE = 1;
const uint8_t DATA 		= 2;
const uint8_t ERROR 	= 3;

const uint8_t SENSOR_NODE_ID = 16;

const uint8_t NAME_SIZE = 10;
const uint8_t MAX_SENSORS = 16;
const uint8_t MAX_PACKET_SIZE = 250;

// Basename for our supported sensors
const char MPU_BASENAME[] 	= "MPU6050_0";
const char LIDAR_BASENAME[]	= "LIDAR_0\0\0";
const char SONAR_BASENAME[]	= "SONAR_0\0\0";
const char RSSI_BASENAME[]  = "RSSI\0\0\0\0\0";

#define packet_handshake_t 	packet_t<sensor_format_t, MAX_SENSORS>
#define packet_data_t 		packet_t<float, (MAX_PACKET_SIZE - 4) / sizeof(float)>
#define packet_error_t 		packet_t<char, MAX_PACKET_SIZE - 2>

// General packet for easy implementaion of new packet format.
template<typename T, uint8_t sz>
struct packet_t {
	uint8_t message_type;
	uint8_t size;
	T payload[sz];
};

// Format for a sensor.
// Its name and how many data points it creates.
struct sensor_format_t {
	char name[NAME_SIZE];
	uint8_t data_count;
};

class Communication {
public:
	// Init communication
	// We find and connect to master then send the data format for the session.
	static bool init();

	// Sends whatever is in the package_data to master.
	static void sendData();

	// Sends whatever is in the package_error to master.
	static void sendErrorMessage();

	// Send handshake package if we want to change the data format
	// during session. (Highly unlikely)
	static bool sendHandshake();

	// Connected flag
	static bool is_connected;

	// Error_flag, sets to 1 if we encountered an error.
	static bool error_flag;
public:
	static packet_handshake_t 	packet_handshake;
	static packet_data_t	 	packet_data;
	static packet_error_t		packet_error;
};


#endif