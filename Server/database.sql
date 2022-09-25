DROP TABLE if EXISTS SUSTest;
CREATE TABLE SUSTest (
  ID INT NOT NULL AUTO_INCREMENT, 
  Question TEXT NOT NULL, 
  PRIMARY KEY(ID)
);

DROP TABLE if EXISTS Answer;
CREATE TABLE Answer (
  Question TEXT NOT NULL, 
  StronglyDisagree bit, 
  Disagree bit,
  Neutral bit, 
  Agree bit, 
  StronglyAgree bit, 
  PRIMARY KEY(ID)
)

INSERT INTO SUSTest(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree) VALUES ("how zufrieden are you?",false, true, false, false, false);