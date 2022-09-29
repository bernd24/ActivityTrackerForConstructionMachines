//#include <Sensor.h>
#include <Communication.h>


#include <Adafruit_MPU6050.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()


void setup() {
  //Wire.begin();
  Serial.begin(115200);
  Communication::initCommunication();
  
  /*
  if(!mpu1.begin()) {
    while(1) {
      Serial.println("Could not find MPU1");
      delay(500);
    };
  }
  */
}

void loop() {


  delay(1000);

/*
  // In the loop we scan for slave
  ScanForSlave();
  // If Slave is found, it would be populate in `slave` variable
  // We will check if `slave` is defined and then we proceed further
  if (slave.channel == CHANNEL) { // check if slave channel is defined
    // `slave` is defined
    // Add slave as peer if it has not been added already
    esp_err_t isPaired = manageSlave();
    if (isPaired == ESP_OK) {
      // pair success or already paired
      // Send data to device
      sendData(&Handshake_Packet, sizeof(Handshake_Packet));
    } else {
      // slave pair failed
      Serial.println("Slave pair failed!");
    }
  }
  else {
    // No slave found to process
  }


  // wait for 3seconds to run the logic again
  delay(3000);*/
}
