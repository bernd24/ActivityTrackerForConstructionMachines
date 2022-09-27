
DROP TABLE IF EXISTS Measurement;
DROP TABLE IF EXISTS WorkSessionActivity;
DROP TABLE IF EXISTS WorkSession;
DROP TABLE IF EXISTS SensorInstance;
DROP TABLE IF EXISTS Activity;
DROP TABLE IF EXISTS Sensor;
DROP TABLE IF EXISTS SensorNodeInstance;
DROP TABLE IF EXISTS SensorNode;
DROP TABLE IF EXISTS Machine;
DROP TABLE IF EXISTS SensorConfiguration;
DROP TABLE IF EXISTS MachineModel;

CREATE TABLE MachineModel (
  Id INT NOT NULL AUTO_INCREMENT,
  Manufacturer VARCHAR(255),
  ModelName VARCHAR(255),
  MachineType VARCHAR(255),
  PRIMARY KEY (Id)
);
INSERT INTO MachineModel (Manufacturer,ModelName,MachineType) VALUES ("Fredrik","Woodig","Excavator");

CREATE TABLE SensorConfiguration (
  Id INT NOT NULL AUTO_INCREMENT,
  Picture VARCHAR(255),
  PRIMARY KEY (Id)
);

CREATE TABLE Machine (
  Id INT NOT NULL AUTO_INCREMENT,
  M_Id INT NOT NULL,
  C_Id INT,
  FOREIGN KEY (C_Id) REFERENCES SensorConfiguration(Id),
  FOREIGN KEY (M_Id) REFERENCES MachineModel(Id),
  PRIMARY KEY (Id)
);
INSERT INTO Machine (M_Id) VALUES ((SELECT Id FROM MachineModel WHERE Manufacturer = "Fredrik"));

CREATE TABLE SensorNode (
  Id INT NOT NULL AUTO_INCREMENT,
  MAC VARCHAR(255),
  BatteryStatus INT,
  PRIMARY KEY (Id)
);

CREATE TABLE SensorNodeInstance (
  Id INT NOT NULL AUTO_INCREMENT,
  C_Id INT NOT NULL,
  SN_Id INT,
  hasBattery BOOLEAN,
  isMaster BOOLEAN,
  FOREIGN KEY (C_Id) REFERENCES SensorConfiguration(Id),
  FOREIGN KEY (SN_Id) REFERENCES SensorNode(Id),
  PRIMARY KEY (Id)
);

CREATE TABLE Sensor (
  Id INT NOT NULL AUTO_INCREMENT,
  SensorName VARCHAR(255),
  SensorType VARCHAR(255),
  Manufacturer VARCHAR(255),
  PRIMARY KEY (Id)
);

CREATE TABLE Activity (
  Id INT NOT NULL AUTO_INCREMENT,
  ActivityName VARCHAR(255),
  FuelEstimate FLOAT,
  PRIMARY KEY (Id)
);

CREATE TABLE SensorInstance (
  Id INT NOT NULL AUTO_INCREMENT,
  S_Id INT NOT NULL,
  SN_Id INT NOT NULL,
  RssiPartner INT,
  A_Id INT,
  FOREIGN KEY (SN_Id) REFERENCES SensorNodeInstance(Id),
  FOREIGN KEY (S_Id) REFERENCES Sensor(Id),
  FOREIGN KEY (RssiPartner) REFERENCES SensorInstance(Id),
  FOREIGN KEY (A_Id) REFERENCES Activity(Id),
  PRIMARY KEY (Id)
);

CREATE TABLE WorkSession (
  Id INT NOT NULL AUTO_INCREMENT,
  Notes TEXT,
  C_Id INT NOT NULL,
  M_Id INT NOT NULL,
  FOREIGN KEY (C_Id) REFERENCES SensorConfiguration(Id),
  FOREIGN KEY (M_Id) REFERENCES MachineModel(Id),
  PRIMARY KEY (Id)
);

CREATE TABLE WorkSessionActivity (
  Id INT NOT NULL AUTO_INCREMENT,
  StartTime TIMESTAMP,
  EndTime TIMESTAMP,
  WS_Id INT NOT NULL,
  A_Id INT NOT NULL,
  FOREIGN KEY (WS_Id) REFERENCES WorkSession(Id),
  FOREIGN KEY (A_Id) REFERENCES Activity(Id),
  PRIMARY KEY (Id)
);

CREATE TABLE Measurement (
  Id BIGINT NOT NULL AUTO_INCREMENT,
  SensorData FLOAT,
  TimeOfMeasure TIMESTAMP,
  SI_Id INT NOT NULL,
  FOREIGN KEY (SI_Id) REFERENCES SensorInstance(Id),
  PRIMARY KEY (Id) 
);