/**
   ESPNOW - Basic communication - Slave
   Date: 26th September 2017
   Author: Arvind Ravulavaru <https://github.com/arvindr21>
   Purpose: ESPNow Communication between a Master ESP32 and a Slave ESP32
   Description: This sketch consists of the code for the Slave module.
   Resources: (A bit outdated)
   a. https://espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
   b. http://www.esploradores.com/practica-6-conexion-esp-now/

   << This Device Slave >>

   Flow: Master
   Step 1 : ESPNow Init on Master and set it in STA mode
   Step 2 : Start scanning for Slave ESP32 (we have added a prefix of `slave` to the SSID of slave for an easy setup)
   Step 3 : Once found, add Slave as peer
   Step 4 : Register for send callback
   Step 5 : Start Transmitting data from Master to Slave

   Flow: Slave
   Step 1 : ESPNow Init on Slave
   Step 2 : Update the SSID of Slave with a prefix of `slave`
   Step 3 : Set Slave in AP mode
   Step 4 : Register for receive callback and wait for data
   Step 5 : Once data arrives, print it in the serial monitor

   Note: Master and Slave have been defined to easily understand the setup.
         Based on the ESPNOW API, there is no concept of Master and Slave.
         Any devices can act as master or salve.
*/

#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESPNow/Basic/Slave Example");
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);
}

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
}

void loop() {
  // Chill
}
