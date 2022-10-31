#pragma once
#include <string>
#include <vector>

struct Data {
	std::vector<float> data;
	std::string sensorName;
	std::string sensorType;
	std::string sensorAxis;
	std::string si_id;
};