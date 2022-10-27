#ifndef COMMUNICATION_MASTER_H
#define COMMUNICATION_MASTER_H

#include <SIM800L.h>
//#include <esp_now.h>
//#include <WiFi.h>
//#include <esp_wifi.h> // only for esp_wifi_set_channel()
#include <RingBuf.h>
#include <SPIFFS.h>

#define PRINT_DATA 1
#define WRITE_TO_FILE 0
static File file_data;
static bool file_flag = false;

#define CHANNEL 1
#define SIM800_RST_PIN 4
const char APN[] = "online.telia.se";
const char URL2[] = "http://h2986165.stratoserver.net/Session/Data";
const char URL1[] = "http://h2986165.stratoserver.net/Session/Handshake";
//const char URL[] = "https://a00212bf-e590-424f-b2cf-751b9a9d644c.mock.pstmn.io/Machine/Create";
const char CONTENT_TYPE[] = "application/json";



#define MAX_CONNECTIONS 4
#define MASTER_NODE_ID 48

const uint32_t node_id_to_instance_id_map[] = {3,4,2};

typedef void(*OnDataRecieveFunc)(const uint8_t*, const uint8_t*, int);

const uint8_t HANDSHAKE = 1;
const uint8_t DATA 		= 2;
const uint8_t _ERROR 	= 3;

const uint8_t NAME_SIZE = 14;
const uint8_t MAX_SENSORS = 16;
const uint8_t MAX_PACKET_SIZE = 250;


#define packet_handshake_t 	packet_t<sensor_format_t, MAX_SENSORS>
#define packet_data_t 		packet_t<float, 800>
#define packet_error_t 		packet_t<char, MAX_PACKET_SIZE - 2>


// General packet for easy implementaion of new packet format.

template<typename T, uint32_t sz>
struct packet_t {
	uint8_t message_type;
	uint32_t size;
	T payload[sz];

	uint8_t getSensorNodeID() const{
		return message_type & 240;
	}
	uint8_t getMessageType() const{
		return message_type & 15;
	}
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
	RingBuf<packet_handshake_t, 4>		handshake_queue;
	RingBuf<packet_data_t, 		1> 	data_queue;
	RingBuf<packet_error_t, 	32> 	error_queue;
};


const uint16_t JSON_MAX_PAYLOAD = 8000;
struct JSON_data_packet {
	char* payload = new char[JSON_MAX_PAYLOAD];
	uint16_t current_index = 0;
	uint8_t number_of_packets = 0;

	uint8_t node_id = 0;
	bool handshake_flag = false;
	bool is_full = false;

	void initPayload(const packet_data_t& packet);
	bool addDataToPayload(const packet_data_t& packet);

	void finalizePayload();

	void resetPayload();

	static void setIndex(uint8_t id);
	static uint8_t getIndex(uint8_t id);
};

class Communication_Master {
public:
	static bool init(char ssid[] = "Slave_1", char pass[] = "Slave_1_Password", uint8_t channel = CHANNEL, bool ssid_hidden = 0, uint8_t max_connections = MAX_CONNECTIONS);

	static void setOnDataRecvFunction(OnDataRecieveFunc f);

	static bool serverHandshake();

	static void loadPacketIntoJSON(packet_handshake_t packet, char payload[]);
	static void loadPacketIntoJSON(packet_data_t packet, char payload[]);
	static void loadPacketIntoJSON(packet_error_t packet, char payload[]);

	static int16_t sendToServer(const char					payload[]);
	static int16_t sendToServer(const JSON_data_packet& 	packet);
	static int16_t sendToServer(const packet_handshake_t& 	handshake);
	static int16_t sendToServer(const packet_data_t& 	 	data);
	static int16_t sendToServer(const packet_error_t&	 	error);

	static void close();

	static Message_queue queue;

	// To keep track of the data format for connected sensor_nodes
	struct sensor_node {
		uint8_t sensor_node_id;
		sensor_format_t data_format[MAX_SENSORS];
	};

	static sensor_node sensor_node_list[MAX_CONNECTIONS];
	static uint8_t sensor_node_count;

	static bool concatJsonArrays(char big_arr[]);
	static JSON_data_packet server_packet[MAX_CONNECTIONS];
	static char small_packet[300];


	//static char sensor_data[2048][32];
	
private:
	static SIM800L* sim800;

	static bool setupSIM800();
};

#endif