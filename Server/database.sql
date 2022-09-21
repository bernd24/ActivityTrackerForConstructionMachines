DROP TABLE if EXISTS SUSTest;
CREATE TABLE SUSTest (
  ID INT NOT NULL AUTO_INCREMENT, 
  Question TEXT NOT NULL, 
  StronglyDisagree BOOLEAN, 
  Disagree BOOLEAN,
  Neutral BOOLEAN, 
  Agree BOOLEAN, 
  StronglyAgree BOOLEAN, 
  PRIMARY KEY(ID)
);

INSERT INTO SUSTest(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree) VALUES ("how zufrieden are you?",false, true, false, false, false);