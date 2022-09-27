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
const uint8_t HANDSHAKE = 0;
const uint8_t DATA 		= 1;
const uint8_t ERROR 	= 2;

const uint8_t NAME_SIZE = 10;
const uint8_t MAX_SENSORS = 16;
const uint8_t MAX_PACKET_SIZE = 250;

const char MPU_BASENAME[] 	= "MPU6050_0";
const char BNO_BASENAME[] 	= "BNO055_0\0";
const char LIDAR_BASENAME[]	= "LIDAR_0\0\0";
const char SONAR_BASENAME[]	= "SONAR_0\0\0";
const char RSSI_BASENAME[]  = "RSSI\0\0\0\0\0";

#define packet_handshake_t 	packet_t<sensor_format_t, MAX_SENSORS>
#define packet_data_t 		packet_t<float, MAX_PACKET_SIZE - 1>
#define packet_error_t 		packet_t<char, MAX_PACKET_SIZE - 1>


template<typename T, uint8_t sz>
struct packet_t {
	uint8_t message_types;
	uint8_t size;
	T payload[sz];
};

struct sensor_format_t {
	char name[NAME_SIZE];
	uint8_t data_count;
};

class Communication {
public:

	static bool initCommunication();

	//static bool getMacAddress()
	
	/*
	static void createSensorFormatArray(sensor_format_t arr[], uint8_t mpu_count, uint8_t bno_count, uint8_t lidar_count, uint8_t supersonic_count, bool rssi = false);

	static bool initCommunication(bool rssi = false);

	// What error would we be sending?
	//void sendErrorMessage(String message);

	void sendData();
	int32_t ScanForRSSI();

	// Init ESP Now with fallback
	void InitESPNow();


	// Scan for slaves in AP mode
	uint8_t ScanForSlave();

	// Check if the slave is already paired with the master.
	// If not, pair the slave with master
	esp_err_t manageSlave();
	esp_err_t deletePeer();

	// callback when data is sent from Master to Slave
	static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

	static bool setupESPnow();

	static bool connected;
	*/
public:
	static packet_handshake_t 	packet_handshake;
	static packet_data_t	 	packet_data;
	static packet_error_t		packet_error;
};


#endif