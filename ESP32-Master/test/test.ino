#include <esp_now.h>
#include <WiFi.h>
#include <Communication_Master.h>

RingBuf<float, 16> buf;


void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }

  Communication_Master::init();
}
/*
// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  Serial.println("");

  uint8_t type = *data;
  uint8_t sz = *(data + 1);
  Serial.print("Type is : "); Serial.println(type);
  Serial.println("Length of espnow package is: "); Serial.println(data_len);
  char c[10];
  switch (type) {
    case 1:
      Serial.println("Handshake package recieved");
      Serial.print("With size: "); Serial.println(sz);
      for(int j = 0; j < sz; ++j) {
        Serial.print("Sensor name: ");
        for(int i = 0; i < 10; ++i) {
          c[i] = (char)*(data + 2 + i);
          if(c[i] != '\0') Serial.print(c);
        }
      }

      Serial.println("");
      Serial.print("Data count: "); Serial.println(*(data + 12));
      break;
    case 2:
      Serial.println("Recieved data package");
      Serial.print("With size: "); Serial.println(sz);
      for(int i = 0; i < sz; ++i) {
        uint8_t bytes[4];
        bytes[0] = *(data + 4 + i*4);
        bytes[1] = *(data + 5 + i*4);
        bytes[2] = *(data + 6 + i*4);
        bytes[3] = *(data + 7 + i*4);

        float f = 0.0;
        memcpy(&f, &bytes, sizeof(f));
        
        switch (i) {
          case 0:
            Serial.print("Acc x: ");
            break;
          case 1:
            Serial.print("Acc y: ");
            break;
          case 2:
            Serial.print("Acc z: ");
            break;
          case 3:
            Serial.print("Gyro x: ");
            break;
          case 4:
            Serial.print("Gyro y: ");
            break;
          case 5:
            Serial.print("Gyro z: ");
            break;
        }
        Serial.println(f);
      }
  }
}*/

void loop() {
  packet_handshake_t handshake;
  packet_data_t data;
  while(Communication_Master::queue.popHandshake(handshake)) {
    Serial.print("Current handshake package from id: ");
    Serial.println(handshake.message_type & 240);
    for(int i = 0; i < handshake.size; ++i) {
      sensor_format_t format = handshake.payload[i];
      Serial.print(format.name); Serial.print(", "); Serial.println(format.data_count);
    }
  }

  while(Communication_Master::queue.popData(data)) {
    Serial.println("Data packet-------------------");
    Serial.print("sensor_id: "); Serial.println(data.message_type & 240);
    for(int i = 0; i < data.size; ++i) {
      Serial.println(data.payload[i]);
    }
  }

  if(Communication_Master::queue.getDataQueueSize() == 0) {
    Serial.println("Data queue is empty");
  }
  
  delay(1000);
}
