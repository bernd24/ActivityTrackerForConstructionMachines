DELETE FROM SUSTests;
DELETE FROM VisAWITests; 
DELETE FROM UEQTests;
DELETE FROM Testpersons;
INSERT INTO Testpersons(Id, anonymName) VALUES (1,"Testperson1");

-- Includes for the SUSTests
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I think that I would like to use this website frequently.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I found the website unnecessarily complex.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I thought the website was easy to use.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I think that I would need the support of a technical person to be able to use this website.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I found the various functions in this website were well integrated. ",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I thought there was too much inconsistency in this website.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I would imagine that most people would learn to use this website very quickly.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I found the website very cumbersom/awkward to use.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I felt very confident using the website.",false, false, false, false, false,1);
INSERT INTO SUSTests(Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree,TestpersonId) VALUES ("I needed to learn a lot of things before i could get going with this system.",false, false, false, false, false,1);

-- Includes for the VisAWITests
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout appears too dense.",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout is easy to grasp",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("Everything goes together on this site",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The side appears patchy",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout appears well structured",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout is pleasantly varied",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout is inventive",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The design appears uninspired",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout appears dynamic",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The design is uninteresting",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The colour composition is attractive",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The colours do not match",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The choice of colours is botched",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The colours are appealing",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout appears professionally designed.",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The layout is not up-to-date.",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The site is designed with care.",false,false,false, false, false, false, false,1);
INSERT INTO VisAWITests(Question,DisagreeEtAll,Disagree,DisagreeRather, Neutral, AgreeRather ,Agree, AgreeEtAll,TestpersonId) VALUES ("The design of the site lacks a concept.",false,false,false, false, false, false, false,1);

-- Includes for the User UEQTests
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("annoying","enjoyable",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("annoying","enjoyable",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("not understandable","understandable",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("creative","dull",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("easy to learn","diffucult to learn",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("valuable","inferior",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("boring","exciting",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("not interesting","interresting",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("unpredictable","predictable",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("fast","slow",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("inventive","conventional",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("obstructive","supportive",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("good","bad",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("complicated","easy",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("unlikable","pleasing",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("usual","leading edge",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("unpleasant","pleasant",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("secure","not secure",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("motivating","demotivating",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("meets expectations","does not meet expectations",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("inefficient","efficient",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("clear","confusing",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("impractical","practical",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("organized","cluttered",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("attractive","unattractive",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("friendly","unfriendly",false,false,false, false, false, false, false,1);
INSERT INTO UEQTests(LeftWord, RightWord,one,two,three,four,five,six,seven,TestpersonId) VALUES ("conservative","innovative",false,false,false, false, false, false, false,1);


