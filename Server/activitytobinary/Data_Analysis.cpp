// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// Data_Analysis.cpp, 10-10-2022, 28-10-2022
// Implementaion of algorithms for detecting movement from sensor data.

#include "Data_Analysis.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <deque>

#include <fstream>

// If we want to use moving window
#define MOVING 0

std::vector<Activity_Timeline> predict(const std::vector<Data>& data)
{
	const int window_size = 600;
	const float std_dev = 0.5;
	const int start = 18;
	const int softstop = 12;
	const int hardstop = 9;
	const float cmp = 0.6;

	std::vector<Activity_Timeline> ats;
	// Go through each dataset and create a timeline for it
	for (Data d : data) {

		if (d.sensorName == "HC_SR04") {
			d.data = derivative(d.data);
			lowpass_filter_2(d.data, 4);
		}

		else if (d.sensorName == "MPU6050" && d.sensorType == "Accelerometer") {
			d.data = derivative(d.data);
		}

		Activity_Timeline at = convert_to_raw_binary(d.sensorName, d.data, window_size, std_dev);
		at.post_process(start, softstop, hardstop);
		at.compress(10, cmp);

		ats.push_back(at);
	}

	return ats;
}

// Converts all values outside threshold to 1's and those inside threshold to 0's
Activity_Timeline convert_to_raw_binary(std::string name, const float * begin, const float * end, size_t window_size, float perc)
{
	std::deque<float> moving_arr;
#if MOVING
	for (int i = 0; i < window_size; ++i) {
		if (i < window_size / 2) {
			moving_arr.push_back(*begin);
		}
		else
			moving_arr.push_back(*(begin + i - window_size / 2));
	}
	double old_avg;
	double avg;
	double old_var;
	double var;
	double std_dev;
	avg = average_mean(moving_arr.begin(), moving_arr.end());
	var = variance(moving_arr.begin(), moving_arr.end(), avg);
	std_dev = 1.2 * sqrt(var);
#else
	// Calculate Average and Standard deviation
	double avg = average_mean(begin, end);
	double std_dev = perc *my_std_dev(begin, end);
#endif

	std::vector<bool> timeline;
	size_t size = end - begin;
	for (int i = 0; i < size; ++i) {
#if MOVING
		float old_value = moving_arr.front();
		moving_arr.pop_front();
		float new_value = *(begin + i + window_size / 2);
		moving_arr.push_back(new_value);

		old_avg = avg;
		old_var = var;

		avg = old_avg + (new_value - old_value) / window_size;
		var = old_var + (new_value - avg + old_value - old_avg)*
			(new_value - old_value) / (window_size - 1);
		std_dev = 1.2 * sqrt(var);
#endif
		
		float value = *(begin + i);

		// Check wether value exists outside or inside given thresholds.
		bool is_outside = value > (avg + std_dev) || value < (avg - std_dev);
		timeline.push_back(is_outside);
	}

	Activity_Timeline a(name, timeline);
		
	return a;
}

Activity_Timeline convert_to_raw_binary(std::string name, const std::vector<float>& vec, size_t window_size, float perc)
{
	return convert_to_raw_binary(name, &vec.front(), &vec.back() + 1, window_size, perc);
}

Activity_Timeline convert_to_binary(const float * begin, const float * end, const conversion_format & format)
{
	return convert_to_binary(begin, end, format.hardstop_bias, format.window_size, format.std_percentage, format.start_bias, format.softstop_bias);
}

// An older verison of the model, where we also parse for continous movement to filter out uninteresting movement.
Activity_Timeline convert_to_binary(const float * begin, const float * end, size_t resolution, size_t window_size, float perc, size_t start_bias, size_t stop_bias)
{
	size_t length = window_size;
	std::vector<float> moving_arr;
	for (int i = 0; i < length; ++i) {
		if (i < length / 2)
			moving_arr.push_back(*begin);
		else
			moving_arr.push_back(*(begin + i - length / 2));
	}
	
#if MOVING
	float avg = average_mean(&moving_arr.front(), &moving_arr.back() + 1);
	float std_dev = my_std_dev(&moving_arr.front(), &moving_arr.back() + 1);
#else
	float avg = average_mean(begin, end);
	float std_dev = perc * my_std_dev(begin, end);
#endif

	const size_t hz_rate = 10;

	size_t sz = end - begin;
	
	std::vector<std::pair<int, int>> ranges;
	int start;
	int stop;
	bool trying_to_start = false;
	bool trying_to_stop = false;
	bool has_started = false;
	bool has_stopped = false;

	int start_timer = 0;
	int stop_timer = 0;

	int trying_to_start_timer = 0;

	for (size_t i = 0; i < sz; ++i) {
		float value = (*(begin + i));
#if MOVING
		
		moving_arr.erase(moving_arr.begin());
		if (i + length / 2 < sz)
			moving_arr.push_back(*(begin + i + length / 2));
		else
			moving_arr.push_back(*(end - 1));

		avg = average_mean(&moving_arr.front(), &moving_arr.back() + 1);
		std_dev = my_std_dev(&moving_arr.front(), &moving_arr.back() + 1);
#endif
		// We recognize movement
		if (value > avg + std_dev || value < avg - std_dev) {
			// If we have started and go above stddev we reset stop timer
			if (trying_to_stop) {
				trying_to_stop = false;
				stop_timer = 0;
			}
			if (has_started) {
				stop_timer = 0;
			}
			else if (trying_to_start) {
				++start_timer;
			}
			else if (!trying_to_start) {
				start = i / hz_rate;
				trying_to_start = true;
				++start_timer;
			}
		}
		if (start_timer > start_bias) {
			has_started = true;
			start_timer = 0;
		}
		// "No movement"
		if (value < avg + std_dev && value > avg - std_dev) {
			// The first time we dip below the std_dev after being active we set stop marker.
			if (has_started && !trying_to_stop) {
				stop = i / hz_rate;
				trying_to_stop = true;
				++stop_timer;
			}
			else if (trying_to_stop) {
				++stop_timer;
			}
			// If we are trying to start and go below stddev we reset
			else if (trying_to_start) {
				if (trying_to_start_timer > stop_bias) {
					trying_to_start = false;
					start_timer = 0;
					trying_to_start_timer = 0;
				}
				++trying_to_start_timer;
			}
		}
		if (stop_timer > resolution) {
			has_stopped = true;
		}
		// If we have stopped a range we push into vector and reset everything
		if (has_stopped) {
			ranges.push_back(std::make_pair(start, stop));
			start_timer = 0;
			stop_timer = 0;
			trying_to_start_timer = 0;
			trying_to_start = false;
			trying_to_stop = false;
			has_started = false;
			has_stopped = false;
		}
	}
	if (ranges.size() == 0) {
		return Activity_Timeline();
	}

	Activity_Timeline timeline("Digging", ranges.back().second, &ranges.front(), &ranges.back() + 1);

	return timeline;
}

Activity_Timeline naive_convert_postprocess(const float * begin, const float * end)
{
	float avg = average_mean(begin, end);
	float std_dev = my_std_dev(begin, end);

	std::vector<std::pair<int, int>> ranges;

	size_t size = end - begin;

	int start, stop;

	bool start_is_set = false;
	bool stop_is_set = false;

	for (size_t i = 0; i < size; ++i) {
		float value = *(begin + i);
		if (value > avg + std_dev || value < avg - std_dev) {
			if (!start_is_set) {
				start = i / 1;
				start_is_set = true;
			}
		}
		else {
			if (start_is_set) {
				stop = i / 1;
				stop_is_set = true;
			}
		}

		if (stop_is_set) {
			ranges.push_back(std::make_pair(start, stop));
			start_is_set = false;
			stop_is_set = false;
		}
	}
	for (auto p : ranges) {
		std::cout << p.first << " -> " << p.second << "\n";
	}
	if (ranges.size() == 0) {
		return Activity_Timeline();
	}
	Activity_Timeline t("Piston", ranges.back().second, &ranges.front(), &ranges.back() + 1);
	return t;
}

// Given an array of sensor data and target timeline, this will find the best parameter for that array to be converted into a timeline
// This is the training algorithm for our model.
conversion_format get_best_format_bruteforce(const std::vector<float>& data, const Activity_Timeline & target)
{
	Activity_Timeline temp;
	conversion_format format;
	float best = 0;

	const float std_dev_step = 0.05;
	const float window_step = 100;
	const int start_limit = 17;
	const int softstop_limit = 17;
	const int hardstop_limit = 35;
	const int compress_limit = 10;

	for (int threshold = 5; threshold < 22; ++threshold) {
		temp = convert_to_raw_binary("Data", data, threshold * window_step, threshold * std_dev_step);
		for (int start = 0; start < start_limit; ++start) {
			for (int softstop = 0; softstop < softstop_limit; ++softstop) {
				for (int hardstop = 1; hardstop < hardstop_limit; ++hardstop) {
					Activity_Timeline node = temp;
					node.post_process(start, softstop, hardstop);
					for (int compress = 1; compress < compress_limit; ++compress) {
						Activity_Timeline n = node;
						n.compress(10, compress * 0.1);
						
						float acc = n.mcc(target);
						if (acc > best) {
							best = acc;
							std::cout << "Acc: " << acc << "\n";
							format.window_size = threshold * window_step;
							format.std_percentage = threshold * std_dev_step;
							format.start_bias = start;
							format.softstop_bias = softstop;
							format.hardstop_bias = hardstop;
							format.compress_percentage = compress * 0.1;
						}
					}
				}
			}
		}
	}

	return format;
}

// Doesn't work, fix me please
conversion_format get_best_format_bruteforce(const std::vector<std::vector<float>>& data, const Activity_Timeline & target)
{
	std::vector<Activity_Timeline> temp_arr(data.size());
	conversion_format format;
	float best = 0;

	const float std_dev_step = 0.1;
	const float window_step = 100;
	const int start_limit = 20;
	const int softstop_limit = 20;
	const int hardstop_limit = 40;
	const int compress_limit = 10;

	for (int threshold = 1; threshold < 15; ++threshold) {
		for (int i = 0; i < data.size(); ++i) {
			temp_arr[i] = convert_to_raw_binary("test", data[i], threshold * window_step, threshold * std_dev_step);
		}
		for (int start = 0; start < start_limit; ++start) {
			for (int softstop = 0; softstop < softstop_limit; ++softstop) {
				for (int hardstop = 1; hardstop < hardstop_limit; ++hardstop) {
					for (int compress = 1; compress < compress_limit; ++compress) {
						Activity_Timeline node;
						for (int i = 0; i < data.size(); ++i) {
							temp_arr[i].post_process(start, softstop, hardstop);
							temp_arr[i].compress(10, compress * 0.1);
						}

						node = temp_arr;

						float acc = node.mcc(target);
						if (acc > best) {
							best = acc;
							std::cout << "Acc: " << acc << "\n";
							format.window_size = threshold * window_step;
							format.std_percentage = threshold * std_dev_step;
							format.start_bias = start;
							format.softstop_bias = softstop;
							format.hardstop_bias = hardstop;
							format.compress_percentage = compress * 0.1;
						}
					}
				}
			}
		}
	}

	return format;
}

// Returns mean average of range
float average_mean(const float * begin, const float * end)
{
	float total = 0.0f;
	size_t counter = 0;

	for (; begin != end; ++begin) {
		total += *begin;
		++counter;
	}

	return total / counter;
}

// Returns mean average from deque range
float average_mean(const std::deque<float>::iterator begin, const std::deque<float>::iterator end)
{
	float total = 0.0f;
	size_t counter = 0;
	for (auto it = begin; it != end; ++it) {
		total += *it;
		++counter;
	}
	return total / counter;
}

// Returns median from range
float average_median(const float * begin, const float * end)
{
	std::vector<float> copy;
	for (; begin != end; ++begin) {
		copy.push_back(*begin);
	}

	std::sort(copy.begin(), copy.end());
	float middle = copy[(copy.size() - 1) / 2];
	return middle;
}

// Returns the mean average magnitude (absolute value)
float average_magnitude(const float * begin, const float * end)
{
	float total = 0.0f;
	size_t counter = 0;
	for (auto it = begin; it != end; ++it) {
		total += abs(*it);
		++counter;
	}
	return total / counter;
}

// Returns the standard deviation in regard to the mean average from range
float my_std_dev(const float * begin, const float * end)
{
	float total_dev = 0.0f;
	size_t counter = 0;
	float avg = average_mean(begin, end);

	for (; begin != end; ++begin) {
		total_dev += pow(avg - *begin, 2);
		++counter;
	}

	return sqrt(total_dev / counter);
}
// Return variance in regard to mean average from deque range
double variance(const std::deque<float>::iterator begin, const std::deque<float>::iterator end, float avg)
{
	double total_dev = 0.0f;
	size_t counter = 0;

	for (auto it = begin; it != end; ++it) {
		total_dev += pow(avg - *it, 2);
		++counter;
	}

	return total_dev / counter;
}

float max3_avg(const float * begin, const float * end)
{
	std::vector<float> copy;
	for (auto it = begin; it != end; ++it) {
		copy.push_back(*it);
	}

	float total = 0;
	for (int i = 0; i < 3; ++i) {
		auto ptr = std::max_element(copy.begin(), copy.end());
		total += *ptr;
		copy.erase(ptr);
	}
	return total / 3;
}

float min3_avg(const float * begin, const float * end)
{
	std::vector<float> copy;
	for (auto it = begin; it != end; ++it) {
		copy.push_back(*it);
	}

	float total = 0;
	for (int i = 0; i < 3; ++i) {
		auto ptr = std::min_element(copy.begin(), copy.end());
		total += *ptr;
		copy.erase(ptr);
	}
	return total / 3;
}

void lowpass_filter_mean(std::vector<float>& vec, size_t window_size)
{
	std::deque<float> moving_arr;
	for (int i = 0; i < window_size; ++i) {
		if (i < window_size / 2) {
			moving_arr.push_back(vec[0]);
		}
		else
			moving_arr.push_back(vec[i - window_size / 2]);
	}
	float old_avg;
	float avg = average_mean(moving_arr.begin(), moving_arr.end());

	std::vector<float> lp;
	for (int i = 0; i < vec.size() - window_size / 2; ++i) {
		float old_value = moving_arr.front();
		moving_arr.pop_front();
		float new_value = vec[i + window_size / 2];
		moving_arr.push_back(new_value);

		old_avg = avg;

		avg = old_avg + (new_value - old_value) / window_size;

		lp.push_back(avg);
	}

	std::swap(lp, vec);
}

// Filtering algorithm for averaging elements.
void lowpass_filter_2(std::vector<float>& vec, size_t window_size)
{
	std::deque<float> moving_arr;

	float old_avg = 0;
	float avg = 0;

	std::vector<float> lp;
	int i = 0;
	for (; i < vec.size() - window_size / 2; ++i) {
		
		float new_value = vec[i + window_size / 2];
		if (moving_arr.size() == window_size) {
			float old_value = moving_arr.front();
			float new_value = vec[i + window_size / 2];
			moving_arr.pop_front();
			moving_arr.push_back(new_value);

			old_avg = avg;

			avg = old_avg + (new_value - old_value) / window_size;

			lp.push_back(avg);
		}
		else {
			moving_arr.push_back(new_value);
			old_avg = avg;
			avg = average_mean(moving_arr.begin(), moving_arr.end());
			lp.push_back(avg);
		}
		
	}
	
	for (int i = 0; i < window_size / 2; ++i) {
		avg = average_mean(moving_arr.begin(), moving_arr.end());
		lp.push_back(avg);
		moving_arr.pop_front();
	}

	std::swap(lp, vec);
}

std::vector<float> derivative(const std::vector<float>& arr) {
	std::vector<float> der;
	if (arr.size() == 0)
		return der;

	for (int i = 0; i < arr.size() - 1; ++i) {
		der.push_back(arr[i] - arr[i + 1]);
	}
	return der;
}

enum LOGICAL_OPERATOR {
	OR, AND, XOR
};

struct logical_OR_object {
	Activity_Timeline operator() (bool invert1, const Activity_Timeline& a1, bool invert2, const Activity_Timeline& a2) {
		int i = 0;
		Activity_Timeline a1_or_a2;
		while (i < a1.size() && i < a2.size()) {
			if (a1.timeline[i] == !invert1 || a2.timeline[i] == !invert2) {
				a1_or_a2.timeline.push_back(true);
			}
			else
				a1_or_a2.timeline.push_back(false);

			++i;
		}
		while (i < a1.size())
			a1_or_a2.timeline.push_back(a1.timeline[i++] == !invert1);

		while (i < a2.size())
			a1_or_a2.timeline.push_back(a2.timeline[i++] == !invert2);

		return a1_or_a2;
	}
};

struct logical_AND_object {
	Activity_Timeline operator() (bool invert1, const Activity_Timeline& a1, bool invert2, const Activity_Timeline& a2) {
		int i = 0;
		Activity_Timeline a1_or_a2;
		while (i < a1.size() && i < a2.size()) {
			if (a1.timeline[i] == !invert1 && a2.timeline[i] == !invert2) {
				a1_or_a2.timeline.push_back(true);
			}
			else
				a1_or_a2.timeline.push_back(false);

			++i;
		}
		while (i < a1.size()) {
			a1_or_a2.timeline.push_back(false);
			++i;
		}

		while (i < a2.size()) {
			a1_or_a2.timeline.push_back(false);
			++i;
		}

		return a1_or_a2;
	}
};

struct logical_XOR_object {
	Activity_Timeline operator() (bool invert1, const Activity_Timeline& a1, bool invert2, const Activity_Timeline& a2) {
		int i = 0;
		Activity_Timeline a1_or_a2;
		while (i < a1.size() && i < a2.size()) {
			if ((a1.timeline[i] == !invert1 && !a2.timeline[i]) == !invert2 || (a2.timeline[i] == !invert2 && !a1.timeline[i] == !invert1)) {
				a1_or_a2.timeline.push_back(true);
			}
			else
				a1_or_a2.timeline.push_back(false);

			++i;
		}
		while (i < a1.size())
			a1_or_a2.timeline.push_back(a1.timeline[i++]);

		while (i < a2.size())
			a1_or_a2.timeline.push_back(a2.timeline[i++]);

		return a1_or_a2;
	}
};

struct logical_operator {
	logical_OR_object or_object;
	logical_AND_object and_object;
	logical_XOR_object xor_object;

	Activity_Timeline operator() (bool invert1, const Activity_Timeline& a1, LOGICAL_OPERATOR op, bool invert2, const Activity_Timeline& a2) {
		switch (op) {
		case OR:
			return or_object(invert1, a1, invert2, a2);

		case AND:
			return and_object(invert1, a1, invert2, a2);

		case XOR:
			return xor_object(invert1, a1, invert2, a2);

		default:
			return Activity_Timeline();
		}
	}
};

Activity_Timeline best_fit(const std::vector<Activity_Timeline>& timeline_arr, const Activity_Timeline & target)
{
	logical_operator op;
	float best_acc = 0;
	Activity_Timeline best;

	size_t size = timeline_arr.size();
	
	std::vector<int> index_permutation;
	for (int i = 0; i < size; index_permutation.push_back(i++));

	std::vector<int> operator_permutations(size);
	std::vector<bool> invert_permutations(size);

	for (int choose = size; choose > 0; --choose) {
		std::cout << size - choose << " Out of " << size << "\n";
		size_t operator_perm_nr = pow(3, choose - 1);
		size_t invert_perm_nr = pow(2, choose);
		do
		{
			for (int op_index = 0; op_index < operator_perm_nr; ++op_index)
			{
				// Set operator flags
				for (int i = 0; i < choose; operator_permutations[i++] = (int)(op_index / pow(3, i)) % 3);
				
				for (int invert_index = 0; invert_index < invert_perm_nr; ++invert_index) {
					// Set invert flags
					for (int i = 0; i < choose; invert_permutations[i++] = invert_index & (1 << i));

					Activity_Timeline combined = timeline_arr[index_permutation[0]];
					if (invert_permutations[0]) combined.invert();

					for (int index = 1; index < choose; ++index) {
						combined = op(false, combined, static_cast<LOGICAL_OPERATOR>(operator_permutations[index - 1]), invert_permutations[index], timeline_arr[index]);
						float acc = combined.mcc(target);
						if (acc > best_acc) {
							best_acc = acc;
							best = combined;
						}
					}
				}
			}
			std::reverse(index_permutation.begin() + choose, index_permutation.end());
		} while (std::next_permutation(index_permutation.begin(), index_permutation.end()));
	}

	return best;
}

Activity_Timeline convert(const std::vector<float>& data, float threshold)
{
	std::vector<bool> timeline;

	for (int i = 0; i < data.size(); ++i) {
		bool positive = abs(data[i]) > threshold;
		timeline.push_back(positive);
	}
	Activity_Timeline t("Test", timeline);
	return t;
}

float sigmoid(float z)
{
	return 1 / (1 + exp(-z));
}

std::vector<float> combine_arr(std::vector<float>& vec1, std::vector<float>& vec2, size_t offset)
{
	std::vector<float> combined_arr;
	for (int i = 0; i < offset; ++i) {
		combined_arr.push_back(vec1[i] / 2);
	}

	int i = 0;
	while (i < vec1.size() - offset && i < vec2.size()) {
		combined_arr.push_back((vec1[i + offset] + vec2[i]) / 2);
		++i;
	}
	while (i < vec2.size()) {
		combined_arr.push_back(vec2[i] / 2);
		++i;
	}
	return combined_arr;
}
