#include <esp_now.h>
#include <WiFi.h>
#include <Communication_Master.h>
#include <SPIFFS.h>


void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }
  Communication_Master::init();
}


void loop() {
  packet_handshake_t handshake;
  packet_data_t data;
  while(Communication_Master::queue.popHandshake(handshake)) {
    Communication_Master::loadPacketIntoJSON(handshake, Communication_Master::small_packet);

    Serial.println("------------------HANDSHAKE--------------------");
    Serial.println(Communication_Master::small_packet);
    int16_t result = Communication_Master::sendToServer(handshake);
    if(result == 200) {
      Serial.println("Handshake successfully sent");
      JSON_data_packet::setIndex(handshake.getSensorNodeID());
    }
    else {
      Serial.println("Could not send handshake");
      Serial.print("Error: "); Serial.println(result);
    }
  }

  Serial.println("Now I sleep for 1 seconds");
  delay(1000);
  bool isFull = false;
  while(Communication_Master::queue.popData(data)) {
    if(isFull) break;
    uint8_t index = JSON_data_packet::getIndex(data);
    if(index > MAX_CONNECTIONS - 1) continue;

    if(!Communication_Master::server_packet[index].addDataToPayload(data)) {
      Communication_Master::server_packet[index].finalizePayload();
      isFull = true;
      int16_t result = Communication_Master::sendToServer(Communication_Master::server_packet[index]);
      if(result == 200) {
        Serial.println("Data successfully sent");
        isFull = false;
        Serial.println("Json packet----");
        Serial.println(Communication_Master::server_packet[index].payload);
        Communication_Master::server_packet[index].initPayload(data);
      }
      else {
        Serial.println("Could not send data");
        Serial.println("Json packet----");
        Serial.println(Communication_Master::server_packet[index].payload);
        Serial.print("Error: "); Serial.println(result);
      }
    }
  }
}
