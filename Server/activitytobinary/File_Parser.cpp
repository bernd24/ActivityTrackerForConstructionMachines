// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// File_Parser.cpp, 10-10-2022, 31-10-2022
// Just a file parser.

#include "File_Parser.h"
#include <string>
#include <vector>

bool copy_file_to_array(std::string filename, std::vector<float>& vec)
{
	std::ifstream file(filename);
	if (!file.is_open())
		return false;

	std::string str;
	while (file >> str) {
		vec.push_back(std::stof(str));
	}
	file.close();
	return true;
}
