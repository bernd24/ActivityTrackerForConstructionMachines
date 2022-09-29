#ifndef COMMUNICATION_MASTER_H
#define COMMUNICATION_MASTER_H

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


#define server_packet_data_t	server_packet_t<float, (MAX_PACKET_SIZE - 4) / sizeof(float)> 
#define server_packet_error_t 	server_packet_t<char, MAX_PACKET_SIZE - 2>

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

class message_queue {
public:
	void push(const packet_handshake_t& handshake);
	void push(const packet_data_t& data);
	void push(const packet_error_t& error);

	packet_handshake_t& popHandshake();
	packet_data_t&		popData();
	packet_error_t&		popError();

	uint8_t getHandshakeQueueSize();
	uint8_t getDataQueueSize();
	uint8_t getErrorQueueSize();

private:
	packet_handshake_t	handshake_queue[255];
	packet_data_t 		data_queue[255];
	packet_error_t 		error_queue[255];

	uint8_t handshake_queue_size;
	uint8_t data_queue_size;
	uint8_t error_queue_size;
};


class Communication_Master {
public:
	static bool init(char ssid[] = "Slave_1", char pass[] = "Slave_1_Password", uint8_t channel = CHANNEL, bool ssid_hidden = 0, uint8_t max_connections = MAX_CONNECTIONS);

	static void setOnDataRecvFunction(OnDataRecieveFunc f);

	static bool serverHandshake();

	static void sendToServer(packet_handshake_t* 	handshake);
	static void sendToServer(packet_data_t* 		data);
	static void sendToServer(packet_error_t* 		error);

	static void close();

	struct sensor_node {
		uint8_t sensor_node_id;
		sensor_format_t data_format[MAX_SENSORS];

		static uint8_t sensor_node_count = 0;
		sensor_node(): sensor_node_id(0), data_format[MAX_SENSORS]() {
			++sensor_node_count;
		}
		~sensor_node() {
			--sensor_node_count;
		}
	}

	static sensor_node sensor_node_list[MAX_CONNECTIONS];
}


#endif