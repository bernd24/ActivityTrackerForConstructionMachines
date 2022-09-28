
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
  ModelYear INT,
  PRIMARY KEY (Id)
);
INSERT INTO MachineModel (Manufacturer,ModelName,MachineType,ModelYear) VALUES ("Fredrik","Woodig","Excavator",2022);
INSERT INTO MachineModel (Manufacturer,ModelName,MachineType,ModelYear) VALUES ("Volvo","EC950F","Excavator",2022);

CREATE TABLE SensorConfiguration (
  Id INT NOT NULL AUTO_INCREMENT,
  Picture VARCHAR(255),
  Notes TEXT,
  PRIMARY KEY (Id)
);

INSERT INTO SensorConfiguration (Picture,Notes) VALUES ("woodig1.jpg","first test");

CREATE TABLE Machine (
  Id INT NOT NULL AUTO_INCREMENT,
  M_Id INT NOT NULL,
  C_Id INT,
  InternalId VARCHAR(255),
  inUse BOOLEAN DEFAULT true,
  FOREIGN KEY (C_Id) REFERENCES SensorConfiguration(Id),
  FOREIGN KEY (M_Id) REFERENCES MachineModel(Id),
  PRIMARY KEY (Id)
);

INSERT INTO Machine (M_Id,C_Id,InternalId) VALUES ((SELECT Id FROM MachineModel WHERE Manufacturer = "Fredrik"),1,"Wood Model #1");
INSERT INTO Machine (M_Id,InternalId,inUse) VALUES ((SELECT Id FROM MachineModel WHERE Manufacturer = "Volvo"),"Test Machine #1",false);

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
  Worker VARCHAR(255),
  C_Id INT NOT NULL,
  M_Id INT NOT NULL,
  FOREIGN KEY (C_Id) REFERENCES SensorConfiguration(Id),
  FOREIGN KEY (M_Id) REFERENCES Machine(Id),
  PRIMARY KEY (Id)
);

INSERT INTO WorkSession (Notes,Worker,C_Id,M_Id) VALUES ("test session","Bernd",1,1);

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
  WS_Id INT NOT NULL,
  SI_Id INT NOT NULL,
  Nr INT NOT NULL,
  TimeOfMeasure TIMESTAMP NOT NULL,
  SensorData FLOAT,
  FOREIGN KEY (SI_Id) REFERENCES SensorInstance(Id),
  FOREIGN KEY (WS_Id) REFERENCES WorkSession(Id),
  PRIMARY KEY (Nr,SI_Id,TimeOfMeasure) 
);