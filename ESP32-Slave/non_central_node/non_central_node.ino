#include <Communication.h>
//#include <Sensor.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()



Adafruit_MPU6050 mpu1;
Adafruit_MPU6050 mpu2;
//Adafruit_BNO055 bno = Adafruit_BNO055(55);
//TwoWire secondWire = TwoWire(0);


void setup() {
  //Wire.begin();
  Serial.begin(115200);
  setupESPnow();
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
  Serial.println("Hello");

  createSensorFormatArray(Handshake_Packet.format_array, 3, 2, 1, 1);

  /*
  createSensorFormatArray(format_array, 3, 2, 1, 1);

for(uint8_t i = 0; i < MAX_SENSORS; ++i) {
    sensor_format_t format = format_array[i];
    Serial.println("Printing global array");
    if(format.name[0] == '\0') break;

    Serial.print("Position "); Serial.print(i); Serial.println(": ");
    Serial.print("Name: "); Serial.println(format.name);
    Serial.print("Count: "); Serial.println(format.data_count);
  }
  for(uint8_t i = 0; i < size; ++i) {
    sensor_format_t format = Handshake_Packet.format_array[i];
    Serial.println("Printing handshake packet");
    if(format.name[0] == '\0') break;

    Serial.print("Position "); Serial.print(i); Serial.println(": ");
    Serial.print("Name: "); Serial.println(format.name);
    Serial.print("Count: "); Serial.println(format.data_count);
  }

  Serial.println("This is the global array sizes");
  Serial.print("One format size -> "); Serial.println(sizeof(format_array[0]));
  Serial.print("Array size      -> "); Serial.println(sizeof(format_array));

*/
  Serial.println("This is the handshake packet:------");
  Serial.print("packet size -> "); Serial.println(sizeof(Handshake_Packet));
  Serial.print("Size of message_type -> "); Serial.println(sizeof(Handshake_Packet.message_type));
  Serial.print("Size of one format -> "); Serial.println(sizeof(Handshake_Packet.format_array[0]));
  Serial.print("Size of format_array -> "); Serial.println(sizeof(Handshake_Packet.format_array));

  
  Serial.println(sizeof(struct handshake_packet));
  Serial.println(sizeof(struct sensor_format_t));
  delay(10000);

  uint8_t* ptr = (uint8_t*)&Handshake_Packet;

  uint8_t m = ptr[0];

  Serial.println(m);
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
