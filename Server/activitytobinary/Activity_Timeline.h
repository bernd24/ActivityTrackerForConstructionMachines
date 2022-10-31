// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// Activity_Timeline.h, 10-10-2022, 31-10-2022
// Activity Timeline represents a binary timeline for an activity using a vector of bool
// Where each bit represent activity in some time period.

#ifndef ACTIVITY_TIMELIME_H
#define ACTIVITY_TIMELIME_H

#include <vector>
#include <string>



// A Binary timeline
class Activity_Timeline {
public:
	Activity_Timeline() = default;
	Activity_Timeline(std::string name, size_t size);
	// Creates a timeline with 1's between given ranges
	Activity_Timeline(std::string name, size_t size, const std::pair<int, int>* begin, const std::pair<int, int>* end);
	Activity_Timeline(std::string name, std::vector<bool> t);
	// Creates a new timeline object by combining to timelines using OR operator.
	Activity_Timeline(const Activity_Timeline& t1, const Activity_Timeline& t2);

	// Fills given ranges in timeline with ones
	void fill_ranges(const std::pair<int, int>* begin, const std::pair<int, int>* end);

	// Shifts the timeline to the right.
	// Pushes false to the front.
	void operator>>(size_t steps);

	void operator=(const std::vector<Activity_Timeline>& arr);

	// Ways of determining the similarity of a timeline to a target timeline
	// OBD Accuracy is precision. How "precise" our ones are to the target
	float obd_accuracy(const Activity_Timeline& target) const;
	// Activity Accuracy is Recall. How many ones do we capture that the target timeline has.
	float activity_accuracy(const Activity_Timeline& target) const;
	// Bit accuracy is just bit by bit accuracy.
	float bit_accuracy(const Activity_Timeline& target) const;
	// Calculates the amount of ones and compares to eachother
	float time_accuracy(const Activity_Timeline& target) const;

	// Matthew's Correlation Coefficient
	// We use this as our primary way to determine a good match
	float mcc(const Activity_Timeline& target) const;
	float fscore(const Activity_Timeline& target) const;

	// Filter using a given timeline
	void filter(const Activity_Timeline& target);

	// filter using AND. Both timelines must have a one in the same place.
	void filter_and(const Activity_Timeline& target);

	// Inverts a timeline. Just a "NOT" operator
	void invert();

	// Post processes the binary timeline. 
	// This will merge ones according to the parameters.
	void post_process(size_t start_threshold = 3, size_t stop_start_threshold = 3, size_t stop_started_threshold = 15);

	// Fill gaps between ones
	void smooth_over(size_t max = 1);

	// Compresses a timeline using factor. 
	// For every segment that we compress we look at how many ones there are 
	// to determine if the compressed segment should become a one using the percentage argument
	void compress(size_t factor = 10, float perc = 0.4);

	// Scales a timeline up by a factor.
	void scale(size_t factor = 10);

	// Calculate the amount of ones
	size_t get_total_time() const;
	// Prints the timeline.
	void print(size_t rowsize = 10) const;

	void print_acc(const Activity_Timeline& target) const;

	// Gets the size of the underlying timeline vector
	size_t size() const;

	// Prints timeline to a file.
	void printToFile(std::string filePath) const;

public:
	std::string activity_name;
	std::vector<bool> timeline;
};


#endif
