#include <esp_now.h>
#include <WiFi.h>

#include "Communication_Master.h"

Communication_Master::sensor_node Communication_Master::sensor_node_list[MAX_CONNECTIONS];
uint8_t Communication_Master::sensor_node_count = 0;

Message_queue Communication_Master::queue;

char server_payload[1024];
uint16_t size = 0;

bool Message_queue::push(const packet_handshake_t& handshake) {
	return handshake_queue.push(handshake);
}

bool Message_queue::push(const packet_data_t& data) {
	return data_queue.push(data);
}

bool Message_queue::push(const packet_error_t& error) {
	return error_queue.push(error);
}

bool Message_queue::popHandshake(packet_handshake_t& handshake) {
	return handshake_queue.pop(handshake);
}

bool Message_queue::popData(packet_data_t& data) {
	return data_queue.pop(data);
}

bool Message_queue::popError(packet_error_t& error) {
	return error_queue.pop(error);
}

uint8_t Message_queue::getHandshakeQueueSize() {
	return handshake_queue.size();
}
uint8_t Message_queue::getDataQueueSize() {
	return data_queue.size();
}
uint8_t Message_queue::getErrorQueueSize() {
	return error_queue.size();
}

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
	// So we do bitwise AND, with 00001111 to remove first 4 bits
	// to get message type.
	uint8_t type = (15 & *data);
	// For the node_id we do the same but with bitstring 11110000.
	uint8_t node_id = (240 & *data);

	uint8_t i;
	for(i = 0; i < Communication_Master::sensor_node_count; ++i) {
		if((Communication_Master::sensor_node_list[i].sensor_node_id == node_id))
			break;
	}
	if(i == Communication_Master::sensor_node_count) {
		Communication_Master::sensor_node_list[i].sensor_node_id = node_id;
		++Communication_Master::sensor_node_count;
	}

	

	// Check what is the message type
	// If new handshake we save the dataformat
	// Otherwise we push to queue.
	switch (type) {
		case HANDSHAKE:
			Communication_Master::queue.push(*(packet_handshake_t*)data);
			copyArray(Communication_Master::sensor_node_list[i].data_format, (sensor_format_t*)(data + 2), *(data + 1));
			break;
		case DATA:
			Communication_Master::queue.push(*(packet_data_t*)data);
			break;
		case ERROR:
			Communication_Master::queue.push(*(packet_error_t*)data);
			break;
	}

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

void loadHandshakePacketIntoJSON(packet_handshake_t& handshake, char payload[]) {
	payload[0] = '{';
	char sensor_str[] = "sensor_node_id:";
}
void Communication_Master::sendToServer(packet_handshake_t* handshake) {
	loadHandshakePacketIntoJSON(handshake, server_payload);
}

void Communication_Master::close() {
	WiFi.scanDelete();
}

void Communication_Master::setOnDataRecvFunction(OnDataRecieveFunc f) {
	esp_now_register_recv_cb(f);
}