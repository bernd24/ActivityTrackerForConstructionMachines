// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// File_Parser.h, 10-10-2022, 31-10-2022
// Just a file parser, for reading floats into array.

#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <fstream>
#include <vector>

// Copy a file containing floats to a vector of floats
bool copy_file_to_array(std::string filename, std::vector<float>& vec);

#endif