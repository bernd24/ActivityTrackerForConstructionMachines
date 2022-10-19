#pragma once
#include "Activity_Timeline.h"
#include <iostream>
#include <vector>
#include <deque>
#include "Data.h"

struct conversion_format {
	size_t window_size = 100;
	float std_percentage = 1.0f;
	size_t start_bias = 3;
	size_t stop_bias = 3;
	size_t hardstop_bias = 18;

	size_t shift = 7;

	void print() {
		std::cout << "Windows size: " << window_size << "\n";
		std::cout << "Std_dev percentage: " << std_percentage << "\n";
		std::cout << "Start bias: " << start_bias << "\n";
		std::cout << "Stop bias: " << stop_bias << "\n";
		std::cout << "Hardstop bias: " << hardstop_bias << "\n";
		std::cout << "Shift: " << shift << "\n";
	}
};

Activity_Timeline convert_to_raw_binary(std::string name, float* begin, const float* end, size_t window_size = 500, float perc = 1.0f);
Activity_Timeline convert_to_raw_binary(std::string name, std::vector<float>& vec, size_t window_size = 500, float perc = 1.0f);

Activity_Timeline convert_to_binary(const float* begin, const float* end, const conversion_format& format);
Activity_Timeline convert_to_binary(const float* begin, const float* end, size_t resolution = 5, size_t window_size = 100, float perc = 1.0f, 
									size_t start_bias = 3, size_t stop_bias = 3);
Activity_Timeline naive_convert_postprocess(const float* begin, const float* end);

conversion_format get_best_format_bruteforce(const std::vector<float> data, const Activity_Timeline& target);

float average_mean(const float* begin, const float* end);
float average_mean(const std::deque<float>::iterator begin, const std::deque<float>::iterator end);
float my_std_dev(const float* begin, const float* end);
double variance(const std::deque<float>::iterator begin, const std::deque<float>::iterator end, float avg);

std::vector<float> derivative(const std::vector<float>& arr);

Activity_Timeline best_fit(const Activity_Timeline& a1, const Activity_Timeline& a2, const Activity_Timeline& a3, const Activity_Timeline& target);

std::vector<Activity_Timeline> predict(std::vector<Data> data);