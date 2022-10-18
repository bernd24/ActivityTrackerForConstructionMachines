#include "Activity_Timeline.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algortihm>

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

float Activity_Timeline::segment_accuracy(const Activity_Timeline & target)
{
	std::vector<std::pair<int, int>> my_ranges;
	std::vector<std::pair<int, int>> target_ranges;

	bool start = false;
	for (int i = 0; i < size(); ++i) {
		if (timeline[i] == true) {
			start = true;
		}
		//else if(ti,e)
	}
	return 0.0;
}

float Activity_Timeline::bit_accuracy(const Activity_Timeline & target)
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

float Activity_Timeline::time_accuracy(const Activity_Timeline & target)
{
	return (float)target.get_total_time() / (float)get_total_time();
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

void Activity_Timeline::post_process(size_t consecutive_bit_length)
{
	int counter = 0;
	int start = 0;
	for (int i = 0; i < size(); ++i) {
		if (timeline[i]) {
			if (counter == 0)
				counter = 2;
			else if (start) {
				for (int k = start; k < i; ++k) {
					timeline[k].flip();
				}
			}
			++counter;
		}
		else {
			if (counter != 0) {
				if (!start) {
					start = i;
				}
				--counter;
			}
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
}

void Activity_Timeline::compress(size_t factor)
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
		if (counter > std::ceil(0.2 * factor)) {
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

size_t Activity_Timeline::size() const
{
	return timeline.size();
}

void Activity_Timeline::printToFile(std::string filePath) const {
	std::ofstream out(filePath);
	for(bool b: timeline){
		out << b << " ";
	}
	out.close();
}
