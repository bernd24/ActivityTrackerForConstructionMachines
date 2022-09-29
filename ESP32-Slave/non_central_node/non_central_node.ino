#include <Sensors.h>
#include <Communication.h>


#define SEND_ERRORS 0


void setup() {
  //Wire.begin();
  Serial.begin(115200);
  // Initialize sensors
  
  while(!Sensors::init(false)) {
    Serial.println("ERROR: Could not find any sensors during initialization");
    delay(1000);
  }

  Serial.print("MPU count: "); Serial.println(Sensors::getMPUSensorCount());
  Serial.print("Lidar count: "); Serial.println(Sensors::getLIDARSensorCount());
  Serial.print("Sonar count: "); Serial.println(Sensors::getSONARSensorCount());
  Serial.print("RSSI count: "); Serial.println(Sensors::getRSSIFlag());

  while(!Communication::init()) {
    Serial.println("ERROR: Could not initialize communication");
    delay(5000);
  }
}

void loop() {
  if(Communication::is_connected) {
    // Get Sensor data and load into the communication data packet
    Communication::packet_data.size = Sensors::getData(Communication::packet_data.payload);

    // Send our data packet.
    Communication::sendData();

  /*
    if(Communication::error_flag) {
      Communication::sendErrorMessage();
    }*/
    
  }


  delay(3000);
}
