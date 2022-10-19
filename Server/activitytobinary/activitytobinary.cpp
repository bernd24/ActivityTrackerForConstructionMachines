// activitytobinary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <filesystem>
#include <cstring>

#include "Activity_Timeline.h"
#include "Data_Analysis.h"
#include "File_Parser.h"
#include "Data.h"

int main(int argc, char *argv[]){

	std::vector<Data> data;
    for (const auto & entry : std::filesystem::directory_iterator(argv[1])){

		char filePath[255];
		std::strcpy(filePath,entry.path().string().c_str());
		std::strtok(filePath,"/");

		Data d;
		d.sensorName = std::string(std::strtok(NULL,"_"));
		d.sensorType = std::string(std::strtok(NULL,"_"));
		d.sensorAxis = std::string(std::strtok(NULL,"_"));
		d.si_id = std::string(std::strtok(NULL,"."));
		
		data.push_back(d);
		
	}
		
	std::vector<Activity_Timeline> ats = predict(data);

	int i = 0;
	for (const auto & entry : std::filesystem::directory_iterator(argv[1])){
		ats[i].printToFile(std::string(argv[1]) + "/" + entry.path().stem().string() + "_timeline.txt");
		i++;
	}

	return 0;
}