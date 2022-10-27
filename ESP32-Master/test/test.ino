#include <Arduino.h>
//#include <esp_now.h>
//#include <WiFi.h>
//#include <SPIFFS.h>
#include <Communication_Master.h>
//#include <SPIFFS.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "BluetoothSerial.h"
#include "ELMduino.h"

BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define SIM800 false

ELM327 myELM327;
float rpm = 0;
float engineload=1;
float enginespeed=2;
float fuellevel= 3;
float fuelrate=4;
float distance = 5;
char* big_packet = new char[1024];

#define READ_FROM_FILE 0

Adafruit_MPU6050 mpu;
bool mpu_flag = true;
uint64_t timer;
const uint64_t SERVER_SEND_RATE = 10000;
packet_data_t master_data;


File file_read;

void getGyro(float* handle, uint16_t index) {
  sensors_event_t a, b, c;
  mpu.getEvent(&a, &b, &c);

  handle[index++] = b.gyro.x;
  handle[index++] = b.gyro.y;
  handle[index++] = b.gyro.z;
}

char speedCommand[20] = {'\0'};
char fuelRateCommand[20] = {'\0'};
char fuelLevelCommand[20] = {'\0'};
char loadCommand[20] = {'\0'};
char rpmCommand[20] = {'\0'};
char distanceCommand[20] = {'\0'};

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }

  ELM_PORT.begin("ArduHUD", true);
  SerialBT.setPin("1234");
  if (!ELM_PORT.connect("OBDII"))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 1");
    //while(1);
  }

  /*if (!myELM327.begin(ELM_PORT, false, 2000))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    //while (1);
  }*/

  if (!myELM327.begin(ELM_PORT, true, 2000, SAE_J1939_29_BIT_250_KBAUD))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");

  if(mpu.begin()) {
    mpu_flag = true;
  }
  else
    Serial.println("No mpu found");

  /*if(READ_FROM_FILE) {
    if(!SPIFFS.begin(true)) {
      Serial.println("Error mounting filesystem for reading");
    }
  }*/
#if SIM800
  Communication_Master::init();
  delay(100);

  Communication_Master::server_packet[0].number_of_packets = 0;
  Communication_Master::server_packet[0].current_index = 0;
  master_data.size = 9;
  master_data.message_type = MASTER_NODE_ID;
  timer = millis();
#endif

  sprintf(speedCommand, MONITOR_FOR_PGN, "FEF1");
  sprintf(fuelRateCommand, MONITOR_FOR_PGN, "FEF2");
  sprintf(fuelLevelCommand, MONITOR_FOR_PGN, "FEFC");
  sprintf(loadCommand, MONITOR_FOR_PGN, "F003");
  sprintf(rpmCommand, MONITOR_FOR_PGN, "F004");
  sprintf(distanceCommand, MONITOR_FOR_PGN, "FEC1");
}

char file_arr[1024] = {'\0'};

void extractData(){
 //while (myELM327.get_response() == ELM_GETTING_MSG || strlen(myELM327.payload) < 2){};
 myELM327.get_response();
  Serial.println(myELM327.payload);
  //strcpy(myELM327.payload,"60FEEE008CFFFFFFFFFFFFFF");
  int base = 0;
  char value[] = {'0','0','\0'};;
  if(base+1 < strlen(myELM327.payload)){
    value[0] = myELM327.payload[base+0];
    value[1] = myELM327.payload[base+1];
    myELM327.responseByte_0 = strtol(value,NULL,16);
  }
  if(base+3 < strlen(myELM327.payload)){
    value[0] = myELM327.payload[base+2];
    value[1] = myELM327.payload[base+3];
    myELM327.responseByte_1 = strtol(value,NULL,16);
  }
  if(base+5 < strlen(myELM327.payload)){
    value[0] = myELM327.payload[base+4];
    value[1] = myELM327.payload[base+5];
    myELM327.responseByte_2 = strtol(value,NULL,16);
  }
  if(base+7 < strlen(myELM327.payload)){
    value[0] = myELM327.payload[base+6];
    value[1] = myELM327.payload[base+7];
    myELM327.responseByte_3 = strtol(value,NULL,16);
  }
  if(base+9 < strlen(myELM327.payload)){
    value[0] = myELM327.payload[base+8];
    value[1] = myELM327.payload[base+9];
    myELM327.responseByte_4 = strtol(value,NULL,16);
  }
  myELM327.sendCommand_Blocking("x");
}

void loop() {
  //float tempRPM = myELM327.rpm();
  //float load= myELM327.engineLoad();*/
  //float spe_ed= myELM327.kph();
  /*float flevel= myELM327.fuelLevel();
  float frate= myELM327.fuelRate(); */

  
    //J1939

    //speed: offset=1 byteSize=2 gain=0.00390625
    myELM327.sendCommand_Blocking(speedCommand);
      extractData();
      enginespeed = ((float)((myELM327.responseByte_1 << 0) | (myELM327.responseByte_2 << 8)))*0.00390625f;
      Serial.print("Engine speed in km/hr: "); Serial.println(enginespeed);    
    

    //fuel rate: offset=0 byteSize=2 gain=0.05
    /*if(myELM327.sendCommand_Blocking(fuelRateCommand) == ELM_SUCCESS){
      extractData();
      fuelrate = ((float)((myELM327.responseByte_0 << 0) | (myELM327.responseByte_1 << 8)))*0.05f; 
      Serial.print("Fuel rate in l/hr: "); Serial.println(fuelrate);
    }

    //fuel level: offset=1 byteSize=1 gain=0.40000000000000002
    if(myELM327.sendCommand_Blocking(fuelLevelCommand) == ELM_SUCCESS){
      extractData();
      fuellevel = ((float)myELM327.responseByte_1)*0.40000000000000002f;    
      Serial.print("Fuel level in %: "); Serial.println(fuellevel);
    }

    //engineload: offset=2 byteSize=1
    if(myELM327.sendCommand_Blocking(loadCommand) == ELM_SUCCESS){
      extractData();
      engineload = myELM327.responseByte_2;   
      Serial.print("Engine Load in %: "); Serial.println(engineload);
    }

    //rpm: offset=3 byteSize=2 gain=0.125    
    if(myELM327.sendCommand_Blocking(rpmCommand) == ELM_SUCCESS){
      extractData();
      rpm = ((float)((myELM327.responseByte_3 << 0) | (myELM327.responseByte_4 << 8)))*0.125f;   
      Serial.print("Engine Speed in RPM: "); Serial.println(rpm);
    }

    //mileage: offset=0 byteSize=4 gain=5
    if(myELM327.sendCommand_Blocking(distanceCommand) == ELM_SUCCESS){
      extractData();
      distance = ((myELM327.responseByte_0 << 0) | (myELM327.responseByte_1 << 8) | (myELM327.responseByte_2 << 16) | (myELM327.responseByte_3 << 24))*5;   
      Serial.print("Mileage in m: "); Serial.println(distance);
    }*/

#if !SIM800
    delay(3000);
#endif

  /*if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(rpm);
    engineload= (uint32_t)load;
    Serial.print("Engine Load in %: "); Serial.println(engineload);
    
    enginespeed=(uint32_t)spe_ed;
    Serial.print("Engine speed in km/hr: "); Serial.println(enginespeed);
    fuellevel=(uint32_t)flevel;
    Serial.print("Fuel level in %: "); Serial.println(fuellevel);
    fuelrate=(uint32_t)frate;
    Serial.print("Fuel rate in l/hr: "); Serial.println(fuelrate);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();*/

#if SIM800
  packet_handshake_t handshake;
  packet_data_t data;
  
  while(Communication_Master::queue.popHandshake(handshake)) {
    //Communication_Master::loadPacketIntoJSON(handshake, Communication_Master::small_packet);
    Serial.println("");
    int16_t result = Communication_Master::sendToServer(handshake);
    if(result == 200) {
      Serial.println("Handshake successfully sent");
      uint8_t i = JSON_data_packet::getIndex(handshake.getSensorNodeID());

      if(i > MAX_CONNECTIONS - 1) {
        Serial.println("Error, successful handshake sent to server. But no json buffer was assigned to this node_id???");
        continue;
      }
      Serial.println("Set handshake flag after successful send");
      Communication_Master::server_packet[i].handshake_flag = true;
    }
    else {
      int counter = 0;
      while(result != 200) {
        if(counter > 5) break;
        Serial.println("Could not send handshake");
        Serial.print("Error: "); Serial.println(result);
        counter++;
        delay(1000);
        result = Communication_Master::sendToServer(handshake);
      }
    }
  }

  // Fetch gyro data
  if(mpu_flag) {

    //for(int i = 0; i < 10; ++i) {
      int i = 0;
      getGyro(master_data.payload, i*8);
      master_data.payload[i*8+3] = fuelrate;
      master_data.payload[i*8+4] = enginespeed; 
      master_data.payload[i*8+5] = engineload; 
      master_data.payload[i*8+6] = fuellevel; 
      master_data.payload[i*8+7] = rpm;  
      master_data.payload[i*8+8] = distance;      

    //}
    //Communication_Master::queue.push(master_data);
  }


  
  /*while(Communication_Master::queue.popData(data) && (millis() - timer < SERVER_SEND_RATE)) {
    uint8_t index = JSON_data_packet::getIndex(data.getSensorNodeID());
    if(index > MAX_CONNECTIONS - 1) {
      Serial.println("Error, no json buffer was assigned for this node_id. We throw away packet");
      Serial.print("Index: "); Serial.println(index);
      continue;
    }
    

    if(!Communication_Master::server_packet[index].handshake_flag) {
      Serial.println("No handshake flag set in json array object, fuck");
      continue;
    }

    if(Communication_Master::server_packet[index].is_full) {
      Serial.println("Json array full, we throw away data packet");
      continue;
    }

    if(!Communication_Master::server_packet[index].addDataToPayload(data)) {
      Communication_Master::server_packet[index].finalizePayload();
    }
  }*/

  Communication_Master::server_packet[0].addDataToPayload(master_data);


  bool fail_send = false;
  int counter = 0;

  //while(millis() - timer > SERVER_SEND_RATE) {
    if(!fail_send) {
      Communication_Master::concatJsonArrays(big_packet);
    }
    Serial.println(big_packet);
    int16_t result = Communication_Master::sendToServer(big_packet);
    if(result == 200) {
      Serial.println("Data successfully sent");
      timer = millis();
      counter = 0;
      fail_send = false;

      //Serial.println("Json packet----");
      //Serial.println(big_packet);
    }
    else {
      counter++;
      //if(counter > 1) break;
      Serial.println("Could not send data");
      Serial.println("Json packet----");
      fail_send = true;
      Serial.println(big_packet);
      Serial.print("Error: "); Serial.println(result);
      delay(500);
    }
#endif
  //}
  /*if(READ_FROM_FILE) {
    file_read = SPIFFS.open("/data.txt");
    if(!file_read) Serial.println("Could not read from file");
    else           Serial.println("Reading from file:");
    while(file_read.available()) {
      Serial.write(file_read.read());
    }
    file_read.close();
  }*/
}
