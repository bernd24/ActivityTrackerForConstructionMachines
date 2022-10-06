#include <Sensors.h>
#include <Communication.h>


#define SEND_ERRORS 0
const uint64_t SECOND = 1000;
const uint64_t HZ = 10;


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
    delay(3000);
  }
  delay(1000);
}
float f = 0.0;

void loop() {
  uint64_t timer = millis();
  if(Communication::is_connected) {
    // Get Sensor data and load into the communication data packet
    Communication::packet_data.size = Sensors::getData(Communication::packet_data.payload);
    //if(Sensor)

    //Communication::packet_data.payload[0] = f;
    // Send our data packet.
    Communication::sendData();

  /*
    if(Communication::error_flag) {
      Communication::sendErrorMessage();
    }*/
    //f += 1.0;
  }

  uint64_t elapsed_time = millis() - timer;

  delay((SECOND / HZ) - elapsed_time);
}
