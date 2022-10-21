#include "Data_Analysis.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <deque>
#include <numeric>

#define MOVING 0

Activity_Timeline convert_to_raw_binary(std::string name, float * begin, const float * end, size_t window_size, float perc)
{
	std::deque<float> moving_arr;
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
	std_dev = sqrt(var);

	std::vector<bool> timeline;
	size_t size = end - begin;
	for (int i = 0; i < size; ++i) {
		if (i > window_size) {
			float old_value = moving_arr.front();
			moving_arr.pop_front();
			float new_value = *(begin + i + window_size / 2);
			moving_arr.push_back(new_value);

			old_avg = avg;
			old_var = var;

			avg = old_avg + (new_value - old_value) / window_size;
			var = old_var + (new_value - avg + old_value - old_avg)*
				(new_value - old_value) / (window_size - 1);
			std_dev = sqrt(var);
		}
		
		float value = *(begin + i);
		if (value > (avg + std_dev) || value < (avg - std_dev)) {
			timeline.push_back(true);
		}
		else
			timeline.push_back(false);
	}

	Activity_Timeline a(name, timeline);
	if (name == "Predicted node3")
		a >> 70;
	else if (name == "Predicted node4")
		a >> 100;

	return a;
}

Activity_Timeline convert_to_raw_binary(std::string name, std::vector<float>& vec, size_t window_size, float perc)
{
	return convert_to_raw_binary(name, &vec.front(), &vec.back() + 1, window_size, perc);
}

Activity_Timeline convert_to_binary(const float * begin, const float * end, const conversion_format & format)
{
	return convert_to_binary(begin, end, format.hardstop_bias, format.window_size, format.std_percentage, format.start_bias, format.stop_bias);
}

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

conversion_format get_best_format_bruteforce(const std::vector<float> data, const Activity_Timeline & target)
{
	Activity_Timeline node;
	std::vector<float> node_der = data;// derivative(data);
	conversion_format format;
	float best = 0;
	int index = 0;
	int m = 0;
	int shift = 0;
	int start = 0;
	int stop = 0;
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 7; ++j) {
			for (int k = 0; k < 5; ++k) {
				node = convert_to_binary(&node_der.front(), &node_der.back() + 1, 1 + j * 3, 50 + i * 20, i * 0.02, 1 + k, 1 + k);
				for (int l = 0; l < 10; ++l) {
					node >> 1;
					//node.smooth_over();
					float acc = node.bit_accuracy(target);
					if (acc > best) {
						best = acc;
						format.window_size = 50 + i * 20;
						format.std_percentage = i * 0.02;
						format.hardstop_bias = j * 3 + 1;
						format.shift = k;
						format.start_bias = format.stop_bias = 1 + l;
						std::cout << "New best: " << acc << "\n";
						std::cout << "Time acc: " << node.time_accuracy(target) << "\n";
					}
				}
			}
		}
		std::cout << "Current progress: " << i << "%\n";
	}
	std::cout << "Best acc: " << best << ", at index: " << index << ", and m: " << m << ", and shift: " << shift << ", and start: " << start << "\n";
	Activity_Timeline n = convert_to_binary(&node_der.front(), &node_der.back() + 1, format);
	n >> shift;
	target.print();
	n.print();
	return format;
}

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

std::vector<float> derivative(const std::vector<float>& arr) {
	std::vector<float> der;

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

Activity_Timeline best_fit(const Activity_Timeline & a1, const Activity_Timeline & a2, const Activity_Timeline & a3, const Activity_Timeline & target)
{
	Activity_Timeline timeline_arr[3] = { a1, a2, a3 };
	logical_operator op;
	float best_acc = 0;
	std::string index;
	std::string oper;
	std::string invert;
	Activity_Timeline best;
	
	int index_permutation[] = { 0, 1, 2 };
	int operator_enum_permutation[] = { 0, 1, 2 };
	
	do
	{
		for(int op_index = 0; op_index < 9; ++op_index)
		{
			for (int i = 0; i < 8; ++i) {
				bool first_inverted = i & 4;
				bool second_inverted = i & 2;
				bool third_inverted = i & 1;
				Activity_Timeline combined = op(first_inverted, timeline_arr[index_permutation[0]], static_cast<LOGICAL_OPERATOR>(op_index / 3), second_inverted, timeline_arr[index_permutation[1]]);
				combined = op(false, combined, static_cast<LOGICAL_OPERATOR>(op_index % 3), third_inverted, timeline_arr[index_permutation[2]]);
				float acc = combined.bit_accuracy(target);
				if (acc > best_acc) {
					best_acc = acc;
					best = combined;
					std::cout << first_inverted << "-" << index_permutation[0] << " " << op_index / 3 << " " << second_inverted << "-" << index_permutation[1];
					std::cout << " " << op_index % 3 << " " << third_inverted << "-" << index_permutation[2] << "\n";
				}
			}
		}
	} while (std::next_permutation(index_permutation, index_permutation + 3));

	return best;
}

float average(std::vector<float> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

std::vector<Activity_Timeline> predict(std::vector<Data> data){
	std::vector<Activity_Timeline> ats;
	for(Data d: data){
		Activity_Timeline at;
		at.timeline = std::vector<bool>();
		if(d.sensorName == "MPU6050" && d.sensorType == "Gyrometer" && d.sensorAxis == "Z"){
			/*float last5[] = {0,0,0,0,0};
			int i = 0;
			bool digging = false;
			int index = 0;
			for(float f: d.data){
				last5[i] = std::abs(f);
				i++; 
				if(i == 5){
					if(last5[0] > 0.06f && last5[1] > 0.06f && last5[2] > 0.06f && last5[3] > 0.06f && last5[4] > 0.06f){
						digging = true;
						at.timeline[index-1] = true;
						at.timeline[index-2] = true;
						at.timeline[index-3] = true;
						at.timeline[index-4] = true;
					}
					else if(last5[0] < 0.06f && last5[1] < 0.06f && last5[2] < 0.06f && last5[3] < 0.06f && last5[4] < 0.06f){
						digging = false;
						at.timeline[index-1] = false;
						at.timeline[index-2] = false;
						at.timeline[index-3] = false;
						at.timeline[index-4] = false;
					}
					i = 0;
				}
				at.timeline.push_back(digging);
				index++;
			}*/

			for(int i = 0; i < d.data.size(); i++){
				std::vector<float> window((i-3 < 0 ? d.data.begin() : d.data.begin()+i-3), (i+3 >= d.data.size() ? d.data.end() : d.data.begin()+i+3));
				if(average(window) < 0.06){
					at.timeline.push_back(false);
				}
				else{
					at.timeline.push_back(true);
				}
			}
		}
		ats.push_back(at);
	}
	return ats;
}