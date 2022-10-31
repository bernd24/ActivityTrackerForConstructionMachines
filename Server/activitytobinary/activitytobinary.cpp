// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// activitytobinary.cpp, 10-10-2022, 31-10-2022
// Main function file, takes command line arguments for which files should be parsed
// and converted into timelines.

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstring>

#include "Activity_Timeline.h"
#include "Data_Analysis.h"
#include "File_Parser.h"
#include "test.h"
#include "Data.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Too few arguments\n";
		return 1;
	}

	std::vector<Data> data;
	for (const auto & entry : std::filesystem::directory_iterator(argv[1])) {

		char filePath[255];
		std::strcpy(filePath, entry.path().string().c_str());
		std::strtok(filePath, "/");

		Data d;
		std::cout << entry.path().string() << std::endl;
		float f;
		// Copy data from file.
		copy_file_to_array(entry.path().string(), d.data);
		d.sensorName = std::string(std::strtok(NULL, "_"));
		d.sensorType = std::string(std::strtok(NULL, "_"));
		d.sensorAxis = std::string(std::strtok(NULL, "_"));
		d.si_id = std::string(std::strtok(NULL, "."));

		data.push_back(d);
	}

	std::vector<Activity_Timeline> ats = predict(data);

	int i = 0;
	for (const auto & entry : std::filesystem::directory_iterator(argv[1])) {
		ats[i].printToFile(std::string(argv[1]) + "/" + entry.path().stem().string() + "_timeline.txt");
		++i;
	}

	return 0;
}
