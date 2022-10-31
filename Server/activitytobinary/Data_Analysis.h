// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// Data_Analysis.h, 10-10-2022, 31-10-2022
// Data analysis algorithms for predicting an activity timeline using an array of sensor data.

#ifndef DATA_ANALYSIS_H
#define DATA_ANALYSIS_H

#include "Activity_Timeline.h"
#include <iostream>
#include <vector>
#include <deque>

#include "Data.h"

struct conversion_format {
	// Threshold
	unsigned int window_size;
	float std_percentage;

	// Bias for starting and stopping activity
	unsigned int start_bias;
	unsigned int softstop_bias;
	unsigned int hardstop_bias;

	// Percentage of ones required for the compressed segment to be considered a one.
	float compress_percentage;

	conversion_format(): window_size(0), std_percentage(0), start_bias(0), softstop_bias(0), hardstop_bias(0), compress_percentage(0) {}

	void print() {
		std::cout << "Windows size: " << window_size << "\n";
		std::cout << "Std_dev percentage: " << std_percentage << "\n";
		std::cout << "Start bias: " << start_bias << "\n";
		std::cout << "Softstop bias: " << softstop_bias << "\n";
		std::cout << "Hardstop bias: " << hardstop_bias << "\n";
		std::cout << "Compress percentage: " << compress_percentage << "\n";
	}
};

// Given an array of datasets, predicts activity timeline for each one.
std::vector<Activity_Timeline> predict(const std::vector<Data>& data);

// Convert data array to a timeline, using the mean average +- the standard deviation as threshold.
// Can use both a "static" percentage of the standard deviation across the entire array
// Or using a sliding window approach, where the threshold (average and standard deviation) is dynamic.
Activity_Timeline convert_to_raw_binary(std::string name, const float* begin, const float* end, size_t window_size = 500, float perc = 1.0f);
Activity_Timeline convert_to_raw_binary(std::string name, const std::vector<float>& vec, size_t window_size = 500, float perc = 1.0f);

// Extra parameters for setting biases for starting and stopping.
// Ex. If you have a singular data point outside the threshold, maybe we shouldnt record it as interesting movement.
Activity_Timeline convert_to_binary(const float* begin, const float* end, const conversion_format& format);
Activity_Timeline convert_to_binary(const float* begin, const float* end, size_t resolution = 5, size_t window_size = 100, float perc = 1.0f, 
									size_t start_bias = 3, size_t stop_bias = 3);
Activity_Timeline naive_convert_postprocess(const float* begin, const float* end);

// Finds the best parameters (within normal range) for converting an array of data into a timeline that is the most similar to the target.
// Uses the most recent approach where we use "convert_to_raw_binary" to produce an activity timeline then
// we post process to remove isolated 1s and merge islands which are close to eachother.
conversion_format get_best_format_bruteforce(const std::vector<float>& data, const Activity_Timeline& target);

/* 
//
// Statistical function for float arrays
*/
// Calculates the mean average of the range.
float average_mean(const float* begin, const float* end);
float average_mean(const std::deque<float>::iterator begin, const std::deque<float>::iterator end);

// Calculates the median of the range.
float average_median(const float* begin, const float* end);

// Calculates the mean magnitude (Absolute value) of range
float average_magnitude(const float* begin, const float* end);

// Calculates the standard deviation using the mean average.
float my_std_dev(const float* begin, const float* end);
// Variance using mean average
double variance(const std::deque<float>::iterator begin, const std::deque<float>::iterator end, float avg);

// Calculate the mean average of the maximum 3 values in range
float max3_avg(const float* begin, const float* end);
// Calculate the mean average of the minimum 3 values in range
float min3_avg(const float* begin, const float* end);

// Reduces noise in array by averageing all datapoints, with eachother, inside a given window size.
void lowpass_filter_mean(std::vector<float>& vec, size_t window_size);
void lowpass_filter_2(std::vector<float>& vec, size_t window_size);

// Calculates the change between elements.
std::vector<float> derivative(const std::vector<float>& arr);

// Finds the best combination of given timelines to produce as high of a match with target as possible.
// VERY SLOW! Tries all possible permutations, should implement a previous result list so we do not calculate duplicates.
Activity_Timeline best_fit(const std::vector<Activity_Timeline>& timeline_arr, const Activity_Timeline& target);

// Converts array to a timeline using a static threshold.
Activity_Timeline convert(const std::vector<float>& data, float threshold);

// Calculates the sigmoid of z.
float sigmoid(float z);

// Combine two arrays by averaging their values.
std::vector<float> combine_arr(std::vector<float>& vec1, std::vector<float>& vec2, size_t offset);

#endif