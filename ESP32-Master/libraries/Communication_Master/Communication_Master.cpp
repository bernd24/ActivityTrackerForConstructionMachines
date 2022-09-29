#include <esp_now.h>
#include <WiFi.h>

sensor_node Communication_Master::sensor_node_list[MAX_CONNECTIONS];
uint8_t Communication_Master::current_count = 0;

bool isSameMac(const uint8_t* mac1, const uint8_t* mac2) {
	for(int i = 0; i < 6; ++i) {
		if(mac1[i] != mac2[i])
			return false;
	}
	return true;
}

template<typename T>
void copyArray(T target[], const T src[], uint8_t size) {
	for(uint8_t i = 0; i < size; ++i) {
		target[i] = src[i];
	}
}

void OnDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
	// First byte contains both sensor_node_id and message type
	// So we do bitwise and with 00001111 to remove first 4 bits
	// to get message type.
	uint8_t type = (15 & *data);
	// For the node_id we do the same but with bitstring 11110000.
	uint8_t node_id = (240 & *data);

	uint8_t i;
	for(i = 0; i < current_count; ++i) {
		if((Communication_Master::sensor_node_list[i].sensor_node_id == node_id))
			break;
	}
	if(i == current_count) {
		Communication_Master::sensor_node_list[i].sensor_node_id = node_id;
	}

	

	// Check what is the message type
	// And copy information to sensor_node object.
	switch (type) {
		case HANDSHAKE:
			copyArray(sensor_node_list[i].data_format, (sensor_format_t*)(data + 2), *(data + 1));
			break;
		case DATA:
			
			break;
		case ERROR:
			break;
	}

	sendToServer();
}

bool Communication_Master::init(char ssid[], char pass[], uint8_t channel, bool ssid_hidden, uint8_t max_connections) {
	WiFi.mode(WIFI_AP);

	if(!WiFi.softAP(ssid, pass, channel, ssid_hidden, max_connections))
		return false;



	WiFi.disconnect();
	if(esp_now_init() != ESP_OK)
		return false;


	esp_now_register_recv_cb(OnDataRecv);
}

void Communication_Master::close() {
	WiFi.scanDelete();
}

void Communication_Master::setOnDataRecvFunction(OnDataRecieveFunc f) {
	esp_now_register_recv_cb(f);
}