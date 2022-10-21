#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <Communication_Master.h>
#include <SPIFFS.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define READ_FROM_FILE 0

Adafruit_MPU6050 mpu;
bool mpu_flag = false;
uint64_t timer;
const uint64_t SERVER_SEND_RATE = 5000;
packet_data_t master_data;

File file_read;

void getGyro(float* handle, uint16_t index) {
  sensors_event_t a, b, c;
  mpu.getEvent(&a, &b, &c);

  handle[index++] = b.gyro.x;
  handle[index++] = b.gyro.y;
  handle[index++] = b.gyro.z;
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }
  if(mpu.begin()) {
    mpu_flag = true;
  }
  else
    Serial.println("No mpu found");

  if(READ_FROM_FILE) {
    if(!SPIFFS.begin(true)) {
      Serial.println("Error mounting filesystem for reading");
    }
  }

  Communication_Master::init();
  delay(100);

  Communication_Master::server_packet[0].number_of_packets = 0;
  Communication_Master::server_packet[0].current_index = 0;
  master_data.size = 30;
  master_data.message_type = MASTER_NODE_ID;
  timer = millis();
}

char file_arr[1024] = {'\0'};

void loop() {
  packet_handshake_t handshake;
  packet_data_t data;
  
  while(Communication_Master::queue.popHandshake(handshake)) {
    //Communication_Master::loadPacketIntoJSON(handshake, Communication_Master::small_packet);
    Serial.println("");
    int16_t result = Communication_Master::sendToServer(handshake);
    if(result == 200) {
      Serial.println("Handshake successfully sent");
      uint8_t i = JSON_data_packet::getIndex(handshake.getSensorNodeID());
      
      if(i > MAX_CONNECTIONS - 1) {
        Serial.println("Error, successful handshake sent to server. But no json buffer was assigned to this node_id???");
        continue;
      }
      Serial.println("Set handshake flag after successful send");
      Communication_Master::server_packet[i].handshake_flag = true;
    }
    else {
      int counter = 0;
      while(result != 200) {
        if(counter > 5) break;
        Serial.println("Could not send handshake");
        Serial.print("Error: "); Serial.println(result);
        counter++;
        delay(1000);
        result = Communication_Master::sendToServer(handshake);
      }
    }
  }

  // Fetch gyro data
  if(mpu_flag) {
    for(int i = 0; i < 10; ++i) {
      getGyro(master_data.payload, i*3);
      delay(100);
    }
    Communication_Master::queue.push(master_data);
  }
  
  while(Communication_Master::queue.popData(data) && (millis() - timer < SERVER_SEND_RATE)) {
    uint8_t index = JSON_data_packet::getIndex(data.getSensorNodeID());
    if(index > MAX_CONNECTIONS - 1) {
      Serial.println("Error, no json buffer was assigned for this node_id. We throw away packet");
      Serial.print("Index: "); Serial.println(index);
      continue;
    }
    

    if(!Communication_Master::server_packet[index].handshake_flag) {
      Serial.println("No handshake flag set in json array object, fuck");
      continue;
    }

    if(Communication_Master::server_packet[index].is_full) {
      Serial.println("Json array full, we throw away data packet");
      continue;
    }

    if(!Communication_Master::server_packet[index].addDataToPayload(data)) {
      Communication_Master::server_packet[index].finalizePayload();
    }
  }

  bool fail_send = false;
  int counter = 0;
  while(millis() - timer > SERVER_SEND_RATE) {
    if(!fail_send) {
      if(!Communication_Master::concatJsonArrays(Communication_Master::big_packet)) break;
    }

    int16_t result = Communication_Master::sendToServer(Communication_Master::big_packet);
    if(result == 200) {
      Serial.println("Data successfully sent");
      timer = millis();
      counter = 0;
      fail_send = false;
      Serial.println("Json packet----");
      Serial.println(Communication_Master::big_packet);
    }
    else {
      counter++;
      if(counter > 1) break;
      Serial.println("Could not send data");
      Serial.println("Json packet----");
      fail_send = true;
      Serial.println(Communication_Master::big_packet);
      Serial.print("Error: "); Serial.println(result);
      delay(500);
    }
  }
  if(READ_FROM_FILE) {
    file_read = SPIFFS.open("/data.txt");
    if(!file_read) Serial.println("Could not read from file");
    else           Serial.println("Reading from file:");
    while(file_read.available()) {
      Serial.write(file_read.read());
    }
    file_read.close();
  }
}
