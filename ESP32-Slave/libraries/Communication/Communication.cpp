
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()

#include "Communication.h"

bool initCommunication(bool rssi) {
  setupESPnow();

  ScanForSlave();
}

// Convert single digit integer to a char.
char intToChar(uint8_t i) {
  if(i > 9)
    return '0';
  return (char)('0' + i);
}

// Copy name from global names. chars arrays are weird.
void copy_name(sensor_format_t& format, const char src[], uint8_t length) {
  for (uint8_t i = 0; i < length; ++i) {
    format.name[i] = src[i];
  }
}

// Creates formats for a given sensor type
void createFormats(sensor_format_t arr[], uint8_t start_index, uint8_t count, uint8_t data_count, const char name[]) {
  for(uint8_t i = 0; i < count; ++i) {
    sensor_format_t format;
    copy_name(format, name, NAME_SIZE);

    uint8_t last_char = NAME_SIZE - 1;
    // Find last non-null character
    while(format.name[last_char] == '\0') { --last_char; }
    format.name[last_char] = intToChar(i);

    format.data_count = data_count;

    arr[start_index + i] = format;
  }
}

void initHandshakeBuffer(uint8_t* buf, sensor_format_t* arr, uint8_t arr_size) {
  buf = (uint8_t*)malloc(1 + sizeof(sensor_format_t) * arr_size);
  buf[0] = HANDSHAKE;

/*
  for(uint8_t i = 0; i < arr_size; ++i) {
    memcpy(buf + 1 + i*sizeof(sensor_format_t), &arr[i], sizeof(sensor_format_t));
  }
  */
}

// Creates the array of formats used for establishing the commmunication data format.
void createSensorFormatArray(sensor_format_t arr[], uint8_t mpu_count, uint8_t bno_count, uint8_t lidar_count, uint8_t supersonic_count) {
  
  // Create mpu formats
  createFormats(arr, 0, mpu_count, 6, MPU_BASENAME);
  // Create bno formats
  createFormats(arr, mpu_count, bno_count, 9, BNO_BASENAME);
  // Create lidar formats
  createFormats(arr, mpu_count + bno_count, lidar_count, 1, LIDAR_BASENAME);
  // Create sonar formats
  createFormats(arr, mpu_count + bno_count + lidar_count, supersonic_count, 2, SONAR_BASENAME);
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

const uint8_t SUCCESS = 0;
const uint8_t NO_SLAVE_FOUND = 1;
const uint8_t NO_WIFI_APS_FOUND = 2;

uint8_t ScanForSlave() {
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL); // Scan only on one channel
  // reset on each scan
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  uint8_t result = 0;

  Serial.println("");
  if (scanResults == 0) {
    result = 2;
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      String BSSIDstr = WiFi.BSSIDstr(i);
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        // Get BSSID => Mac Address of the Slave
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption

        slaveFound = 1;
        // we are planning to have only one slave in this example;
        // Hence, break after we find one, to be a bit efficient
        break;
      }
    }
  }

  if (slaveFound) {
    result = 0;
  } else {
    result = 1;
  }

  // clean up ram
  WiFi.scanDelete();
  return result;
}

esp_err_t manageSlave() {
  if (slave.channel == CHANNEL) {
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if ( exists) {
      // Slave already paired.
      return ESP_OK;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(&slave);
      
      return addStatus;
      /*
      if (addStatus == ESP_OK) {
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
      */
    }
  } else {
    // No slave found to process
    return ESP_FAIL;
  }
}

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

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setupESPnow() {
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  InitESPNow();
  esp_now_register_send_cb(OnDataSent);
}