#include "SIM800L.h"
#include <esp_now.h>
#include <WiFi.h>
#include <cQueue.h>

#define SIM800_RST_PIN 4

const char APN[] = "online.telia.se";
const char URL[] = "http://h2986165.stratoserver.net/Machine/Create";
//const char URL[] = "https://a00212bf-e590-424f-b2cf-751b9a9d644c.mock.pstmn.io/Machine/Create";
const char CONTENT_TYPE[] = "application/json";

#define CHANNEL 1

SIM800L* sim800l;

Queue_t q;

void setup() {
  q_init(&q, sizeof(int32_t), 10, FIFO, false);
  
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);
  while(!Serial);

  // Initialize the hardware Serial1
  Serial2.begin(115200);

  WiFi.mode(WIFI_AP);
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
  }
  Serial.println("MAC-Address: " + WiFi.macAddress() + "\n");
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  delay(1000);
   
  // Initialize SIM800L driver with an internal buffer of 200 bytes and a reception buffer of 512 bytes, debug disabled
  sim800l = new SIM800L((Stream *)&Serial2, SIM800_RST_PIN, 200, 512);

  // Equivalent line with the debug enabled on the Serial
  // sim800l = new SIM800L((Stream *)&Serial1, SIM800_RST_PIN, 200, 512, (Stream *)&Serial);

  // Setup module for GPRS communication
  setupModule();
}

char PAYLOAD[255];

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  q_push(&q,(const int32_t*)incomingData);
}
 
void loop() {
  // Establish GPRS connectivity (5 trials)
  bool connected = false;
  for(uint8_t i = 0; i < 5 && !connected; i++) {
    delay(1000);
    connected = sim800l->connectGPRS();
  }

  // Check if connected, if not reset the module and setup the config again
  if(connected) {
    Serial.print(F("GPRS connected with IP "));
    Serial.println(sim800l->getIP());
  } else {
    Serial.println(F("GPRS not connected !"));
    Serial.println(F("Reset the module."));
    sim800l->reset();
    setupModule();
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  while(1){
    if(!q_isEmpty(&q)){
      uint32_t val;
      char valStr[10];
      strcpy(PAYLOAD,"[");
      q_pop(&q,&val);
      sprintf(valStr,"%d",val);
      strcat(PAYLOAD,valStr);  
      while(q_pop(&q,&val)){
        sprintf(valStr,",%d",val);
        strcat(PAYLOAD,valStr);        
      }
      strcat(PAYLOAD,"]");       
      uint16_t rc = sim800l->doPost(URL, CONTENT_TYPE, PAYLOAD, 10000, 10000);
      if(rc == 200) {
        // Success, output the data received on the serial
        Serial.print(F("HTTP POST successful ("));
        Serial.print(sim800l->getDataSizeReceived());
        Serial.println(F(" bytes)"));
        Serial.print(F("Received : "));
        Serial.println(sim800l->getDataReceived());
      } else {
        // Failed...
        Serial.print(F("HTTP POST error "));
        Serial.println(rc);
      }
    }
    delay(10000);
  }
}

void setupModule() {
    // Wait until the module is ready to accept AT commands
  while(!sim800l->isReady()) {
    Serial.println(F("Problem to initialize AT command, retry in 1 sec"));
    delay(1000);
  }
  Serial.println(F("Setup Complete!"));

  // Wait for the GSM signal
  uint8_t signal = sim800l->getSignal();
  while(signal <= 0) {
    delay(1000);
    signal = sim800l->getSignal();
  }
  Serial.print(F("Signal OK (strenght: "));
  Serial.print(signal);
  Serial.println(F(")"));
  delay(1000);

  // Wait for operator network registration (national or roaming network)
  NetworkRegistration network = sim800l->getRegistrationStatus();
  while(network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
    delay(1000);
    network = sim800l->getRegistrationStatus();
  }
  Serial.println(F("Network registration OK"));
  delay(1000);

  // Setup APN for GPRS configuration
  bool success = sim800l->setupGPRS(APN);
  while(!success) {
    success = sim800l->setupGPRS(APN);
    delay(5000);
  }
  Serial.println(F("GPRS config OK"));
}
