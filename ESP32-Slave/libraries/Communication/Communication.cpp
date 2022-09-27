
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()

#include "Communication.h"
#include "../Sensor/Sensor.h"

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

bool getMacAddress(uint8_t addr[], char ssid_name[] = "Slave") {
  int16_t scan_result = WiFi.scanNetworks(false, false, false, 300, CHANNEL);
  bool result = false;

  for(int16_t i = 0; i < scan_result; ++i) {
    String ssid = WiFi.SSID(i);
    if(ssid.indexOf(ssid_name) == 0) {
      String bssid_str = WiFi.BSSIDstr(i);

      if(6 == sscanf(bssid_str.c_str(), "%x:%x:%x:%x:%x:%x",  
        &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5] )) {
        result = true;
      }
      break;
    }
  }
  WiFi.scanDelete();
  return result;
}


bool connectToSlave(char ssid[] = "Slave") {
  uint8_t mac[6];
  if(!getMacAddress(mac)) {
    return false;
  }

  memset(&slave, 0, sizeof(slave));
  for(int i = 0; i < 6; ++i)
    slave.peer_addr[i] = mac[i];
  
  slave.channel = CHANNEL;
  slave.encrypt = false;

  if(esp_now_is_peer_exist(slave.peer_addr))  return true;
  if(esp_now_add_peer(&slave) == ESP_OK)      return true;

  return false;
}

bool InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    return true;
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
  return false;
}

bool setupESPnow() {
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  bool status = InitESPNow();
  esp_now_register_send_cb(OnDataSent);

  return status;
}

bool Communication::initCommunication() {
  // Inits Wifi and esp-now, also sets channel and cb function
  if(!setupESPnow()) {
    Serial.println("Error initializing esp-now");
    return false;
  }

  // Find and connecto to our slave
  if(!connectToSlave()) {
    Serial.println("Failed to connect to slave");
    return false;
  }

  

  //  OTHER SETUP STUFF?
  createHandshakePacket(packet_handshake);
  if(!send(slave.peer_addr, (uint8_t*)&packet_handshake, 
    (packet_handshake.size * sizeof(sensor_format_t)) + 2)) 
  {
    Serial.println("Error sending handshake");
    return false;
  }

}

// Convert single digit integer to a char.
char intToChar(uint8_t i) {
  if(i > 9)
    return '0';
  return (char)('0' + i);
}

// Copy name from global names. chars arrays are weird.
void copy_name(char target[], const char src[], uint8_t length) {
  for (uint8_t i = 0; i < length; ++i) {
    target[i] = src[i];
  }
}

// Creates formats for a given sensor type
void createFormats(sensor_format_t arr[], uint8_t start_index, uint8_t count, uint8_t data_count, const char name[]) {
  for(uint8_t i = 0; i < count; ++i) {
    sensor_format_t format;
    copy_name(format.name, name, NAME_SIZE);

    // Find and change last non null character.
    uint8_t last_char = NAME_SIZE - 1;
    while(format.name[last_char] == '\0') { --last_char; }
    format.name[last_char] = intToChar(i);

    format.data_count = data_count;

    arr[start_index + i] = format;
  }
}

// Creates the array of formats used for establishing the commmunication data format.
void createHandshakePacket(sensor_format_t arr[]) {
  uint8_t mpu_count         = Sensor::getMPUSensorCount();
  uint8_t bno_count         = Sensor::getBNOSensorCount();
  uint8_t lidar_count       = Sensor::getLIDARSensorCount();
  uint8_t supersonic_count  = Sensor::getSONARSensorCount();
  bool rssi                 = Sensor::getRSSIFlag();

  // Create mpu formats
  createFormats(arr, 0, mpu_count, 6, MPU_BASENAME);
  // Create bno formats
  createFormats(arr, mpu_count, bno_count, 9, BNO_BASENAME);
  // Create lidar formats
  createFormats(arr, mpu_count + bno_count, lidar_count, 1, LIDAR_BASENAME);
  // Create sonar formats
  createFormats(arr, mpu_count + bno_count + lidar_count, supersonic_count, 2, SONAR_BASENAME);

  if(rssi) {
    sensor_format_t format;
    copy_name(format.name, RSSI_BASENAME, NAME_SIZE);
    format.data_count = 1;

    arr[mpu_count + bno_count + lidar_count + supersonic_count] = format;
  }
}


int32_t ScanForRSSI() {
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL); // Scan only on one channel
  if(scanResults == 0) {
    Serial.println("No wifi devices found when scanning for RSSI.");
    WiFi.scanDelete();
    return 0;
  }

  int32_t rssi = 0;
  for(int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    if(SSID.indexOf("Slave") == 0) {
      rssi = WiFi.RSSI(i);
      Serial.print("Measuring RSSI: "); Serial.println(rssi);
      break;
    }
  }

  WiFi.scanDelete();
  return rssi;
}


const uint8_t SUCCESS = 0;
const uint8_t NO_SLAVE_FOUND = 1;
const uint8_t NO_WIFI_APS_FOUND = 2;


esp_err_t deletePeer() {
  esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);

  return delStatus;
  /*
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    // Delete success
    Serial.println("Success");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }*/
}



void sendData(void* buffer, size_t size) {
  const uint8_t *peer_addr = slave.peer_addr;
  Serial.println("Sending: ");
  esp_err_t result = esp_now_send(peer_addr, (uint8_t*)buffer, size);
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}