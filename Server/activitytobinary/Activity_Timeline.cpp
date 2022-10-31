// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// Activity_Timeline.cpp, 10-10-2022, 31-10-2022
// Activity Timeline implementation.

#include "Activity_Timeline.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

Activity_Timeline::Activity_Timeline(std::string name, size_t size): activity_name(name)
{
	for (int i = 0; i < size; ++i) {
		timeline.push_back(false);
	}
}

Activity_Timeline::Activity_Timeline(std::string name, size_t size, const std::pair<int, int>* begin, const std::pair<int, int>* end): Activity_Timeline(name, size)
{
	fill_ranges(begin, end);
}

Activity_Timeline::Activity_Timeline(std::string name, std::vector<bool> t):Activity_Timeline(name, 0)
{
	timeline = t;
}

Activity_Timeline::Activity_Timeline(const Activity_Timeline & t1, const Activity_Timeline & t2): Activity_Timeline(t1.activity_name + t2.activity_name, 0)
{
	int i = 0;
	while (i < t1.size() && i < t2.size()) {
		timeline.push_back(t1.timeline[i] || t2.timeline[i]);
		++i;
	}

	while (i < t1.size()) {
		timeline.push_back(t1.timeline[i]);
		++i;
	}
	while (i < t2.size()) {
		timeline.push_back(t2.timeline[i]);
		++i;
	}
}

void Activity_Timeline::fill_ranges(const std::pair<int, int>* begin, const std::pair<int, int>* end)
{
	for (auto ptr = begin; ptr != end; ++ptr) {
		for (int i = ptr->first; i < ptr->second; ++i) {
			timeline[i].flip();
		}
	}
}

void Activity_Timeline::operator>>(size_t steps)
{
	for (int i = 0; i < steps; ++i) {
		timeline.push_back(false);
	}

	std::rotate(timeline.begin(), timeline.end() - steps, timeline.end());
}

void Activity_Timeline::operator=(const std::vector<Activity_Timeline>& arr)
{
	size_t size = arr[0].size();
	for (int i = 0; i < arr.size(); ++i) {
		if (size != arr[i].size()) {
			std::cout << "Different size\n";
			return;
		}
	}
	timeline.clear();
	timeline.resize(size);
	for (int i = 0; i < size; ++i) {
		bool is_positive = false;
		for (int j = 0; j < arr.size(); ++j) {
			if (arr[j].timeline[i]) {
				is_positive = true;
				break;
			}
		}
		timeline.push_back(is_positive);
	}
}

float Activity_Timeline::obd_accuracy(const Activity_Timeline& target) const
{
	int hits = 0;
	int total = 0;
	for (int i = 0; i < size(); ++i) {
		if (timeline[i] == false)
			continue;

		++total;
		if (i >= target.size())
			continue;
		if (target.timeline[i] == true)
			++hits;
	}
	return (float)hits / total;
}

float Activity_Timeline::activity_accuracy(const Activity_Timeline & target) const
{
	int hits = 0;
	int total = 0;
	for (int i = 0; i < target.size(); ++i) {
		if (target.timeline[i] == false)
			continue;

		++total;
		if (i >= size())
			continue;
		if (timeline[i] == true)
			++hits;
	}
	return (float)hits / total;
}

float Activity_Timeline::bit_accuracy(const Activity_Timeline & target) const
{
	int i = 0;
	int correct = 0;
	while (i < size() && i < target.size()) {
		if (timeline[i] == target.timeline[i]) {
			++correct;
		}
		++i;
	}

	while (i < size()) {
		if (timeline[i] == false) {
			++correct;
		}
		++i;
	}
	while (i < target.size()) {
		if (target.timeline[i] == false) {
			++correct;
		}
		++i;
	}

	return (float)correct / i;
}

float Activity_Timeline::time_accuracy(const Activity_Timeline & target) const
{
	return (float)target.get_total_time() / (float)get_total_time();
}

float Activity_Timeline::mcc(const Activity_Timeline & target) const
{
	size_t TP = 0;
	size_t TN = 0;
	size_t FP = 0;
	size_t FN = 0;

	int i = 0;
	while (i < size() && i < target.size()) {
		if (timeline[i] && target.timeline[i])
			++TP;
		else if (timeline[i] && !target.timeline[i])
			++FP;
		else if (!timeline[i] && !target.timeline[i])
			++TN;
		else if (!timeline[i] && target.timeline[i])
			++FN;
		++i;
	}
	while (i < size()) {
		if (timeline[i])
			++FP;
		else
			++TN;
		++i;
	}
	while (i < target.size()) {
		if (target.timeline[i])
			++FN;
		else
			++TN;
		++i;
	}

	double divisor = sqrt((TP + FP) * (TP + FN) * (TN + FP) * (TN + FN));
	double mcc = (double)(TP * TN) / divisor - (double)(FP * FN) / divisor;
	// Check for overflow, shouldnt happen now.
	if (mcc < -1 || mcc > 1) {
		std::cout << "MCC error: " << mcc << "\n";
		std::cout << "TP: " << TP << "\n";
		std::cout << "TN: " << TN << "\n";
		std::cout << "FP: " << FP << "\n";
		std::cout << "FN: " << FN << "\n";
		return 0.0;
	}
	return mcc;
}

float Activity_Timeline::fscore(const Activity_Timeline & target) const
{
	float precision = obd_accuracy(target);
	float recall = activity_accuracy(target);
	return (2 * precision * recall) / (precision + recall);
}

void Activity_Timeline::filter(const Activity_Timeline & target)
{
	size_t i = 0;
	while (i < size() && i < target.size()) {
		if (timeline[i] == true && target.timeline[i] == true) {
			timeline[i].flip();
		}
		++i;
	}
}

void Activity_Timeline::filter_and(const Activity_Timeline & target)
{
	size_t i = 0;
	while (i < size() && i < target.size()) {
		if (timeline[i] == true && target.timeline[i] == false) {
			timeline[i].flip();
		}
		++i;
	}
}

void Activity_Timeline::invert()
{
	for(auto e : timeline) {
		e.flip();
	}
}

void Activity_Timeline::post_process(size_t start_threshold, size_t stop_start_threshold, size_t stop_started_threshold)
{
	int start_counter = 0;
	int stop_counter = 0;
	int start_index = 0;
	int stop_index = 0;
	bool trying_to_start = false;
	bool started = false;
	bool trying_to_stop = false;

	for (int i = 0; i < size(); ++i) {
		bool is_positive = timeline[i];

		// We are on a one
		if (is_positive) {
			// If we are trying to stop recognizing activity when we reach a one, we reset that timer
			if (trying_to_stop) {
				trying_to_stop = false;
				stop_counter = 0;
			}
			else if (started) {
				stop_counter = 0;
			}
			// If we are trying to start and find a one we increase the start counter
			else if (trying_to_start) {
				++start_counter;
			}
			// If we are not trying to start when we found a one, we try to start.
			else if (!trying_to_start) {
				start_index = i;
				trying_to_start = true;
				++start_counter;
				stop_counter = 0;
			}
			// If start counter is above the minimum threshold we have started recognizing an activity
			if (start_counter >= start_threshold) {
				started = true;
				trying_to_start = false;
				start_counter = 0;
			}
		}
		// We are on a zero
		else if (!is_positive) {
			// If we are recognizing an activity and we are not trying to stop, we will mark index and try to stop.
			if (started && !trying_to_stop) {
				stop_index = i;
				trying_to_stop = true;
				++stop_counter;
			}
			++stop_counter;
		}
		// If we are trying to start, but stop counter is above threshold we discard the activity.
		if (trying_to_start && stop_counter >= stop_start_threshold) {
			start_index = 0;
			trying_to_start = false;
			start_counter = 0;
		}
		// If we have started and the stop counter is above the threshold we will save the activity to the timeline.
		else if (started && stop_counter >= stop_started_threshold) {
			for (int k = start_index; k < stop_index; ++k) {
				if (!timeline[k])
					timeline[k].flip();
			}
			started = false;
		}
	}
}

void Activity_Timeline::smooth_over(size_t max)
{
	bool start_flag = false;
	bool stop_flag = false;
	int counter = 0;
	int i = 0;
	while (i < size()) {
		while (i < size()) {
			if (timeline[i] == false && start_flag) {
				break;
			}
			else if (timeline[i] == true) {
				start_flag = true;
				counter = 0;
			}
			++i;
		}
		while (i < size()) {
			if (timeline[i] == false) {
				++counter;
			}
			else
				break;
			++i;
		}
		if (counter < max) {
			for (int k = 0; k < counter; ++k) {
				timeline[i - k - 1].flip();
			}
		}
	}

	// Remove lonely ones
	for (int i = 2; i < size() - 2; ++i) {
		if (!timeline[i])
			continue;

		bool left = false;
		bool right = false;
		if (!timeline[i - 1] && !timeline[i - 2])
			left = true;
		if (!timeline[i + 1] && !timeline[i + 2])
			right = true;

		if (left && right)
			timeline[i].flip();
	}
}

void Activity_Timeline::compress(size_t factor, float perc)
{
	int start = 0;
	int stop = 0;
	bool start_flag = false;
	bool stop_flag = false;
	std::vector<bool> new_timeline;
	int counter = 0;
	for (int i = 0; i < timeline.size(); i += factor) {
		for (int k = 0; k < factor && k + i < size(); ++k) {
			if (timeline[k + i] == true)
				++counter;
		}
		if (counter > std::ceil(perc * factor)) {
			new_timeline.push_back(true);
		}
		else
			new_timeline.push_back(false);

		counter = 0;
	}

	timeline = new_timeline;
}

void Activity_Timeline::scale(size_t factor)
{
	std::vector<bool> new_timeline;
	for (auto b : timeline) {
		for (int i = 0; i < factor; ++i) {
			new_timeline.push_back(b);
		}
	}
	std::swap(timeline, new_timeline);
}

size_t Activity_Timeline::get_total_time() const
{
	size_t counter = 0;
	
	for (auto b : timeline) {
		if (b)
			++counter;
	}
	return counter;
}

void Activity_Timeline::print(size_t rowsize) const
{
	std::cout << activity_name << ":\n";
	int row_size = 1;
	for (auto b : timeline) {
		std::cout << b;
		if (row_size > rowsize - 1) {
			row_size = 0;
			std::cout << "\n";
		}

		++row_size;
	}
	std::cout << "\n";

	std::cout << "Total time: " << get_total_time() << "\n";
}

void Activity_Timeline::print_acc(const Activity_Timeline& target) const
{
	std::cout << "Activity Name:	" << activity_name << "\n";
	std::cout << "MCC:		" << mcc(target) << "\n";
	std::cout << "Bit Acc:	" << bit_accuracy(target) << "\n";
	std::cout << "Precision:	" << obd_accuracy(target) << "\n";
	std::cout << "Recall:		" << activity_accuracy(target) << "\n";
}

size_t Activity_Timeline::size() const
{
	return timeline.size();
}

void Activity_Timeline::printToFile(std::string filePath) const
{
	std::ofstream out(filePath);
	for (bool b : timeline) {
		out << b << " ";
	}
	out.close();
}
