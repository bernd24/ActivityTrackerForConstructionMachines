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

const uint8_t HANDSHAKE = 0;
const uint8_t DATA 		= 1;
const uint8_t ERROR 	= 2;

const uint8_t NAME_SIZE = 10;
const uint8_t MAX_SENSORS = 16;

const char MPU_BASENAME[] 	= "MPU6050_0";
const char BNO_BASENAME[] 	= "BNO055_0\0";
const char LIDAR_BASENAME[]	= "LIDAR_0\0\0";
const char SONAR_BASENAME[]	= "SONAR_0\0\0";

static uint8_t* handshake_buffer;
static uint8_t* data_buffer;
static uint8_t* error_buffer;

struct sensor_format_t {
	char name[NAME_SIZE] = {'\0'};
	uint8_t data_count;
};

struct handshake_packet {
	uint8_t message_type = HANDSHAKE;
	sensor_format_t format_array[MAX_SENSORS];
};

static handshake_packet Handshake_Packet;
static sensor_format_t format_array[MAX_SENSORS] = {};

char myIntToChar(uint8_t i);

void initHandshakeBuffer(uint8_t* buf, sensor_format_t* arr, uint8_t arr_size);
void createSensorFormatArray(sensor_format_t arr[], uint8_t mpu_count, uint8_t bno_count, uint8_t lidar_count, uint8_t supersonic_count);

bool initCommunication(bool rssi = false);

void sendErrorMessage(String message);

int32_t ScanForRSSI();

// Init ESP Now with fallback
void InitESPNow();


// Scan for slaves in AP mode
uint8_t ScanForSlave();

// Check if the slave is already paired with the master.
// If not, pair the slave with master
esp_err_t manageSlave();
esp_err_t deletePeer();

// send data
//esp_err_t sendPacket(uint8_t message_type, void* buffer, size_t size);
void sendData(void* buffer, size_t size);

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

extern void setupESPnow();


#endif