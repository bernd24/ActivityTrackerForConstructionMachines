#ifndef COMMUNICATION_MASTER_H
#define COMMUNICATION_MASTER_H

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()
#include <RingBuf.h>

#define CHANNEL 1
#define MAX_CONNECTIONS 4
typedef void(*OnDataRecieveFunc)(const uint8_t*, const uint8_t*, int);

const uint8_t HANDSHAKE = 1;
const uint8_t DATA 		= 2;
const uint8_t ERROR 	= 3;

const uint8_t NAME_SIZE = 10;
const uint8_t MAX_SENSORS = 16;
const uint8_t MAX_PACKET_SIZE = 250;

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

class Message_queue {
public:
	Message_queue() = default;

	bool push(const packet_handshake_t& handshake);
	bool push(const packet_data_t&		data);
	bool push(const packet_error_t& 	error);

	bool popHandshake(packet_handshake_t& handshake);
	bool popData(packet_data_t& data);
	bool popError(packet_error_t& error);

	uint8_t getHandshakeQueueSize();
	uint8_t getDataQueueSize();
	uint8_t getErrorQueueSize();

private:
	RingBuf<packet_handshake_t, 16>	handshake_queue;
	RingBuf<packet_data_t, 		64> data_queue;
	RingBuf<packet_error_t, 	64> error_queue;
};



class Communication_Master {
public:
	static bool init(char ssid[] = "Slave_1", char pass[] = "Slave_1_Password", uint8_t channel = CHANNEL, bool ssid_hidden = 0, uint8_t max_connections = MAX_CONNECTIONS);

	static void setOnDataRecvFunction(OnDataRecieveFunc f);

	static bool serverHandshake();

	static void sendToServer(packet_handshake_t* handshake);
	static void sendToServer(packet_data_t* 	 data);
	static void sendToServer(packet_error_t*	 error);

	static void close();

	static Message_queue queue;

	// To keep track of the data format for connected sensor_nodes
	struct sensor_node {
		uint8_t sensor_node_id;
		sensor_format_t data_format[MAX_SENSORS];
	};

	static sensor_node sensor_node_list[MAX_CONNECTIONS];
	static uint8_t sensor_node_count;

private:
	static char server_payload[1024];
	static uint16_t size;
};


#endif