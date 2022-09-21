DROP TABLE IF EXISTS Machine;
CREATE TABLE Machine (
  Id INT NOT NULL AUTO_INCREMENT,
  Manufacturer TEXT,
  Model TEXT,
  MachineType TEXT,
  PRIMARY KEY (Id)
);
INSERT INTO Machine (Manufacturer,Model,MachineType) VALUES ("Volvo","EC950F","Excavator");