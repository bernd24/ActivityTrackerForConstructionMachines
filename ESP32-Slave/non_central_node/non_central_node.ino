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

  if(!Communication::init()) {
    Serial.println("ERROR: Could not initialize communication");
    delay(3000);
  }
}
int32_t i = 0;

void loop() {
  if(Communication::is_connected) {
    // Get Sensor data and load into the communication data packet
    Communication::packet_data.size = Sensors::getData(Communication::packet_data.payload);

    float f = (float)i;

    Communication::packet_data.payload[0] = f;
    // Send our data packet.
    Communication::sendData();

  /*
    if(Communication::error_flag) {
      Communication::sendErrorMessage();
    }*/
    ++i;
  }

  delay(50);
}
