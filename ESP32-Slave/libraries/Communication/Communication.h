#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()

// Global copy of slave
static esp_now_peer_info_t slave;
#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0


//void establish_connection(Format* formats, uint8_t size, bool rssi);

void send_error_message(String message);

int32_t ScanForRSSI();

// Init ESP Now with fallback
void InitESPNow();


// Scan for slaves in AP mode
void ScanForSlave();

// Check if the slave is already paired with the master.
// If not, pair the slave with master
bool manageSlave();
void deletePeer();

// send data
void sendData(void* buffer, size_t size);

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

#endif