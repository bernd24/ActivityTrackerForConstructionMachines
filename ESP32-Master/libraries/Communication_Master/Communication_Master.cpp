//#include <esp_now.h>
//#include <WiFi.h>

#include "Communication_Master.h"
#include <SIM800L.h>

Communication_Master::sensor_node Communication_Master::sensor_node_list[MAX_CONNECTIONS];
uint8_t Communication_Master::sensor_node_count = 0;

Message_queue Communication_Master::queue;

SIM800L* Communication_Master::sim800;
JSON_data_packet Communication_Master::server_packet[MAX_CONNECTIONS];
char Communication_Master::small_packet[] = {'\0'};

//char big_packet[] = {'\0'};


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
void copyArray(T target[], const T src[], uint16_t size) {
	for(uint16_t i = 0; i < size; ++i) {
		target[i] = src[i];
	}
}

char* file_json = new char[16000];
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
			JSON_data_packet::setIndex(node_id);
			copyArray(Communication_Master::sensor_node_list[i].data_format, (sensor_format_t*)(data + 2), *(data + 1));
			break;
		case DATA:
			Serial.println("Recieved data!");
			if(PRINT_DATA) {
				strcpy(file_json, "");
				Communication_Master::loadPacketIntoJSON(*(packet_data_t*)data, file_json);
				Serial.println(file_json);
			}
		/*
			if(file_flag) {
				file_data = SPIFFS.open("/data.txt", FILE_APPEND);
				strcpy(file_json, "");
				Communication_Master::loadPacketIntoJSON(*(packet_data_t*)data, file_json);
				strcat(file_json, "\n");
				if(!file_data.print(file_json)) {
					Serial.println("Couldnt print sensor_data to file");
				}
				file_data.close();
			}
			*/
			Communication_Master::queue.push(*(packet_data_t*)data);
			break;
		case _ERROR:
			Communication_Master::queue.push(*(packet_error_t*)data);
			break;
	}

}

bool Communication_Master::setupSIM800() {
	if(!sim800)
		return false;

	uint32_t start_time = millis();
	uint32_t timeout = 1500000;

	while(!sim800->isReady()) {
		Serial.println("Problem to initialize AT command, retry in 1 sec");
		delay(1000);
		if(millis() - start_time > timeout)
			return false;
	}
	uint8_t signal = sim800->getSignal();
	while(signal < 1) {
		delay(1000);
		Serial.println("No Signal :(");
		signal = sim800->getSignal();
		if(millis() - start_time > timeout)
			return false;
	}

	NetworkRegistration network = sim800->getRegistrationStatus();
	while(network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
		delay(2000);
		Serial.println("NetworkRegistration error");
		network = sim800->getRegistrationStatus();
		if(millis() - start_time > timeout)
			return false;
	}

	bool success = sim800->setupGPRS(APN);
	while(!success) {
		delay(1000);
		Serial.println("GPRS Setup error");
		success = sim800->setupGPRS(APN);
		if(millis() - start_time > timeout)
			return false;
	}

	bool connected = false;
	while(!connected) {
		connected = sim800->connectGPRS();
		Serial.println("Connect GPRS error");
		delay(2000);
		if(millis() - start_time > timeout)
			return false;
	}
	Serial.println("SIM800 setup successful");
	return true;
}

bool Communication_Master::init(char ssid[], char pass[], uint8_t channel, bool ssid_hidden, uint8_t max_connections) {
	/*WiFi.mode(WIFI_AP);

	if(!WiFi.softAP(ssid, pass, channel, ssid_hidden, max_connections))
		return false;


	WiFi.disconnect();
	if(esp_now_init() != ESP_OK)
		return false;*/

	// Setup file as backup
	if(WRITE_TO_FILE) {
		Serial.println("Write mode");
		file_flag = true;
		if(!SPIFFS.begin(true)) {
			Serial.println("Error mounting filesystem");
			file_flag = false;
		}
		file_data = SPIFFS.open("/data.txt", FILE_WRITE);
		if(!file_data) {
			Serial.println("Error opening file");
			file_flag = false;
		}
		if(file_flag) Serial.println("Flag is set to true");
		else 		  Serial.println("Flag is set to FALSE");
		file_data.close();
	}

	Serial2.begin(115200);
	delay(3000);
	sim800 = new SIM800L((Stream*)&Serial2, SIM800_RST_PIN, 1000);
	if(!setupSIM800()) {
		delete sim800;
		return false;
	}
	
	//esp_now_register_recv_cb(OnDataRecv);
	delay(100);

	packet_handshake_t handshake;
	handshake.message_type = MASTER_NODE_ID;
	handshake.size = 7;

	sensor_format_t format;
	snprintf(format.name, sizeof(format.name), "%s", "MPU6050");
	format.data_count = 3;
	handshake.payload[0] = format;

	snprintf(format.name, sizeof(format.name), "%s", "Fuel Rate");
	format.data_count = 1;
	handshake.payload[1] = format;

    snprintf(format.name, sizeof(format.name), "%s", "Vehicle Speed");
	format.data_count = 1;
	handshake.payload[2] = format;

    snprintf(format.name, sizeof(format.name), "%s", "Engine Load");
	format.data_count = 1;
	handshake.payload[3] = format;

    snprintf(format.name, sizeof(format.name), "%s", "Fuel Level");
	format.data_count = 1;
	handshake.payload[4] = format;

    snprintf(format.name, sizeof(format.name), "%s", "RPM");
	format.data_count = 1;
	handshake.payload[5] = format;

    snprintf(format.name, sizeof(format.name), "%s", "Mileage");
	format.data_count = 1;
	handshake.payload[6] = format;

	int16_t result = sendToServer(handshake);
	int counter = 0;
	while(result != 200) {
		if(counter > 5) break;
		delay(500);
		Serial.println("Error while sending master handshake to server!");
		Serial.print("Error: "); Serial.println(result);
		counter++;

		result = sendToServer(handshake);
	}
	if(counter < 6)
		Serial.println("Successful handshake sent");
	else
		Serial.println("We gave up trying to send handshake");

	server_packet[0].node_id = MASTER_NODE_ID;
	server_packet[0].handshake_flag = true;
	server_packet[1].node_id = 16;
	server_packet[2].node_id = 32;
	return true;
}

bool isAllowedChar(const char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ':' || c == '_')
		return true;
	return false;
}

uint16_t getNextInsertionPoint(const char str[], uint16_t index) {
	for(; index > 0; --index) {
		if(isAllowedChar(str[index]) && isAllowedChar(str[index - 1])) {
			return index + 1;
		}
	}
	return 0;
}

void loadJsonAttribute(const char attr_name[], uint8_t name_sz, const char attr_val[], uint8_t val_sz, char payload[], uint16_t& index) {
	strcat(payload, attr_name);
	strcat(payload, attr_val);
	/*
	copyArray(payload + index, attr_name, name_sz);
	index += name_sz - 1;

	copyArray(payload + index, attr_val, val_sz);
	index += val_sz;
	index = getNextInsertionPoint(payload, index);*/
}
// JSON Constants
const char bracket_open 	= '{';
const char bracket_close	= '}';
const char delimiter 		= ',';
const char array_start 		= '[';
const char array_end 		= ']';
const char sensor_str[] 	= "\"node_ID\":";
const char message_str[] 	= "\"type\":";
const char count_str[] 		= "\"count\":";
const char payload_str[] 	= "\"payload\":[";
const char name[] 			= "\"id\":\"";
const char data_count[] 	= "\"elements\":";
const char timestamp[]		= "\"time\":";
const char packets_nr[]		= "\"packets\":";


template<typename T>
void JSONFormat(T packet, char payload[], uint16_t& index) {
	strcat(payload, "{");

	char id_str[4];
	uint8_t id = packet.getSensorNodeID() >> 4;
	uint32_t instance_id = node_id_to_instance_id_map[id - 1];

	snprintf(id_str, sizeof(id_str), "%d", instance_id);
	//snprintf(id_str, sizeof(id_str), "%d", packet.getSensorNodeID());
	loadJsonAttribute(sensor_str, sizeof(sensor_str), id_str, sizeof(id_str), payload, index);

	strcat(payload, ",");
	strcat(payload, payload_str);

	/*
	payload[index++] = bracket_open;

	char id_str[4];
	uint8_t id = packet.getSensorNodeID() >> 4;
	uint32_t instance_id = node_id_to_instance_id_map[id - 1];

	snprintf(id_str, sizeof(id_str), "%d", instance_id);
	//snprintf(id_str, sizeof(id_str), "%d", packet.getSensorNodeID());
	loadJsonAttribute(sensor_str, sizeof(sensor_str), id_str, sizeof(id_str), payload, index);
	payload[index++] = delimiter;

/*	Message Type is not needed because we will use different web-api endpoints for
	the different message types.

	char message_type[3];
	snprintf(message_type, sizeof(message_type), "%d", packet.getMessageType());
	loadJsonAttribute(message_str, sizeof(message_str), message_type, sizeof(message_type), payload, index);
	payload[index++] = delimiter;
	*/

/*	 COUNTS IS NOT NEEDED
	char count[4];
	snprintf(count, sizeof(count), "%d", packet.size);
	loadJsonAttribute(count_str, sizeof(count_str), count, sizeof(count), payload, index);
	payload[index++] = delimiter;
*/
	/*
	copyArray(payload + index, payload_str, sizeof(payload_str));
	index += sizeof(payload_str) - 1;*/
}

void Communication_Master::loadPacketIntoJSON(packet_handshake_t packet, char payload[]) {
	uint16_t index = 0;
	JSONFormat(packet, payload, index);
	strcat(payload, "{");
	for(uint8_t i = 0; i < packet.size; ++i) {
		if(i != 0){
			strcat(payload, ",");//payload[index++] = delimiter;
            strcat(payload, "{");
        }

		char name_str[14];
		copyArray(name_str, packet.payload[i].name, sizeof(packet.payload[i].name));
		loadJsonAttribute(name, sizeof(name), name_str, sizeof(name_str), payload, index);
		strcat(payload, "\",");

		char data_str[3];
		snprintf(data_str, sizeof(data_str), "%d", packet.payload[i].data_count);
		loadJsonAttribute(data_count, sizeof(data_count), data_str, sizeof(data_str), payload, index);
		strcat(payload, "}");
/*
		payload[index++] = bracket_open;
		char name_str[10];
		copyArray(name_str, packet.payload[i].name, sizeof(packet.payload[i].name));
		loadJsonAttribute(name, sizeof(name), name_str, sizeof(name_str), payload, index);
		payload[index++] = '\"';
		payload[index++] = delimiter;


		char data_str[3];
		snprintf(data_str, sizeof(data_str), "%d", packet.payload[i].data_count);
		loadJsonAttribute(data_count, sizeof(data_count), data_str, sizeof(data_str), payload, index);
		payload[index++] = bracket_close;*/
	}
	strcat(payload, "]}");
	/*
	payload[index++] = array_end;
	payload[index++] = bracket_close;*/
}

void Communication_Master::loadPacketIntoJSON(packet_data_t packet, char payload[]) {
	uint16_t index = 0;
	JSONFormat(packet, payload, index);

	for(uint8_t i = 0; i < packet.size; ++i) {
		if(i != 0)
			strcat(payload, ",");

		char data_str[9];  //100.000.000m = 100.000km
		snprintf(data_str, sizeof(data_str), "%f", packet.payload[i]);
		strcat(payload, data_str);
		/*
		copyArray(payload + index, data_str, sizeof(data_str));
		index += sizeof(data_str);
		index = getNextInsertionPoint(payload, index);*/
	}
	strcat(payload, "]}");
	/*
	payload[index++] = array_end;
	payload[index++] = bracket_close;*/
}

void Communication_Master::loadPacketIntoJSON(packet_error_t packet, char payload[]) {
	uint16_t index = 0;
	JSONFormat(packet, payload, index);
}

int16_t Communication_Master::sendToServer(const char payload[]) {
	return sim800->doPost(URL2, CONTENT_TYPE, payload, 10000, 10000);
}

int16_t Communication_Master::sendToServer(const JSON_data_packet& packet) {
	return sim800->doPost(URL2, CONTENT_TYPE, packet.payload, 10000, 10000);
}

int16_t Communication_Master::sendToServer(const packet_handshake_t& handshake) {
	loadPacketIntoJSON(handshake, small_packet);

	int16_t result = sim800->doPost(URL1, CONTENT_TYPE, small_packet, 10000, 10000);
	Serial.println("Inside send to server");
	for(int i = 0; i < 256; ++i) {
		Serial.print(small_packet[i]);
		small_packet[i] = '\0';
	}
	Serial.println("");
	return result;
}

int16_t Communication_Master::sendToServer(const packet_data_t& data) {
	//JSONFormat(data, payload);
}

void Communication_Master::close() {
	//WiFi.scanDelete();
}

void Communication_Master::setOnDataRecvFunction(OnDataRecieveFunc f) {
	//esp_now_register_recv_cb(f);
}

void JSON_data_packet::initPayload(const packet_data_t& packet) {
	resetPayload();
	
	//this->node_id = packet.getSensorNodeID();
	JSONFormat(packet, payload, current_index);
	
	for(uint32_t i = 0; i < packet.size; ++i) {
		if(i != 0)
			strcat(payload, ",");

		char data_str[9];
		snprintf(data_str, sizeof(data_str), "%f", packet.payload[i]);
		if(data_str[7] == '.') data_str[7] = '\0';
		strcat(payload, data_str);
	}
	number_of_packets = 1;
}

bool JSON_data_packet::addDataToPayload(const packet_data_t& packet) {
	// Payload not initialized
	if(number_of_packets < 1) {
		initPayload(packet);
		return true;
	}
	

	// Payload cannot fit the data.
	uint8_t added_data_size = packet.size * 8 + 16;
	uint16_t index_of_null = 0;
	for(uint16_t i = 0; i < JSON_MAX_PAYLOAD; ++i) {
		if(payload[i] == '\0') {
			index_of_null = i;
			break;
		}
	}
	if((JSON_MAX_PAYLOAD - index_of_null) < added_data_size){
		return false;
    }


	for(uint8_t i = 0; i < packet.size; ++i) {
		strcat(payload, ",");
		char data_str[9];
		snprintf(data_str, sizeof(data_str), "%f", packet.payload[i]);
		if(data_str[7] == '.') data_str[7] = '\0';
		strcat(payload, data_str);
	}
	++number_of_packets;
	return true;
}

void JSON_data_packet::finalizePayload() {
	/*
	copyArray(payload + current_index, packets_nr, sizeof(packets_nr));
	current_index += sizeof(packets_nr) - 1;
/*
	char p_nr[4];
	snprintf(p_nr, sizeof(p_nr), "%d", number_of_packets);
	strcat(payload, p_nr);
	*/
	strcat(payload, "]}");
	is_full = true;
}


void JSON_data_packet::resetPayload() {
	for(uint16_t i = 0; i < JSON_MAX_PAYLOAD; ++i) {
		payload[i] = '\0';
	}
	current_index = 0;
	number_of_packets = 0;
	is_full = false;
}

uint8_t JSON_data_packet::getIndex(uint8_t id) {
	uint8_t i = 0;

	for(; i < MAX_CONNECTIONS; ++i) {
		if(Communication_Master::server_packet[i].node_id == id)
			break;
	}

	if(id == 16 && i != 1)
		return 1;
	else if(id == 32 && i != 2)
		return 2;

	if(id == 48 && i != 0) {
		return 0;
	}

	//Serial.println("Returning index automatically");
	return i;
}

void JSON_data_packet::setIndex(uint8_t id) {
	Serial.print("Trying to set index for id: "); Serial.println(id);
	for(uint8_t i = 0; i < MAX_CONNECTIONS; ++i) {
		if(Communication_Master::server_packet[i].node_id == id) {
			Serial.println("There already was a json assigned");
			Communication_Master::server_packet[i].handshake_flag = true;
			return;
		}
	}

	for(uint8_t i = 0; i < MAX_CONNECTIONS; ++i) {
		if(Communication_Master::server_packet[i].node_id == 0) {
			Communication_Master::server_packet[i].node_id = id;
			Communication_Master::server_packet[i].handshake_flag = true;
			return;
		}
	}
	Serial.println("No json packet was 0");
}

bool Communication_Master::concatJsonArrays(char big_arr[]) {
	bool any_packets = false;
	strcpy(big_arr, "[");
	for(int i = 0; i < MAX_CONNECTIONS; ++i) {
		if(server_packet[i].number_of_packets >= 1) {

			if(i != 0)	strcat(big_arr, ",");
			if(!server_packet[i].is_full) server_packet[i].finalizePayload();

			strcat(big_arr, server_packet[i].payload);
			any_packets = true;
			server_packet[i].resetPayload();
		}
	}

	strcat(big_arr, "]");
	if(!any_packets)
		strcpy(big_arr, "");
	return any_packets;
}
/*
int getSensorIDFromJson(const char arr[]) {
	uint8_t i = 0;
	while(i++) {
		if(arr[i] == ',')
			break;
	}
	for(uint8_t j = i - 1; j > 0; --j) {
		if(arr[j] == ':')
			break;
	}

	char id_str[6] = {'\0'};
	copyArray(id_str, arr + j + 1, i - j - 1);

	int id;
	sscanf(id_str, "%d", &id);
	return id;
}

void convertToJson(const packet_data_t& packet) {
	uint8_t index = 0;
	for(; index < 32; ++index) {
		if(getSensorNodeIDFromJson(sensor_data[0][index]) == packet.getSensorNodeID()) {
			break;
		}
	}
	if(index == 32) {
		uint8_t i = 0;
		for(; i < 32; ++i) {
			if(sensor_data[0][i] == '\0')
				break;
		}
		if(i == 32)
			return;

		index = i;
		char s[] = "{\"sensor_id\":";
		strcat_s(sensor_data[0][index]);
	}

	uint8_t i;
	for(i = 0; i < sensor_node_count; ++i) {
		if(sensor_node_list[i].sensor_node_id == packet.getSensorNodeID())
			break;
	}


	for(uint8_t j = 0; j < MAX_SENSORS; ++j) {
		switch(sensor_node_list[i].data_format[j].name) {
			case "MPU6050_0":
			{

			}
			case "SONAR_0":

			case "LIDAR_0":
		}
	}
}*/