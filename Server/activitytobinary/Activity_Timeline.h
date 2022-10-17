#pragma once
#include <vector>

const std::pair<int, int> test1_node3_ranges[]	= { {37, 53}, {77, 78}, {81, 82}, {84, 85}, {94, 98}, {101, 105}, {115, 117}, {120, 121}, {124, 125}, 
													{131, 132}, {182, 183}, {186, 197}, {201, 207}, {209, 213}, {225, 226}, {230, 231}, {233, 234}, {238, 239},
													{242, 245}, {250, 251}, {254, 257}, {262, 263}, {272, 274}, {278, 284}, {289, 290}, {293, 295}, {302, 304} };
const std::pair<int, int> test1_node4_ranges[]	= { {24, 37}, {83, 86}, {93, 97}, {102, 104}, {114, 115}, {123, 125}, {173, 180}, {211, 213}, {219, 223}, {234, 237},
													{245, 246}, {264, 268}, {273, 275}, {307, 310} };

const std::pair<int, int> test1_idle_ranges[]	= { {0, 23}, {131, 173}, {307, 335} };
const std::pair<int, int> test1_dig_ranges[]	= { {23, 54}, {76, 87}, {92, 106}, {113, 127}, {173, 215}, {220, 227}, {232, 248}, {262, 277}, {288, 297} };
const std::pair<int, int> test1_rotate_ranges[] = { {54, 76}, {87, 92}, {106, 113}, {127, 131}, {215, 220}, {227, 232}, {248, 262}, {277, 288}, {297, 307} };

const std::pair<int, int> test2_idle_ranges[]	= { {0, 94}, {240, 263}, {382, 401} };
const std::pair<int, int> test2_dig_ranges[]	= { {102, 112}, {119, 145}, {153, 167}, {173, 187}, {192, 200}, {204, 214}, {221, 234}, {263, 283}, {291, 303}, {309, 324},
													{330, 344}, {350, 362}, {367, 377}, {401, 405}, {410, 421}, {439, 445}, {452, 460} };
const std::pair<int, int> test2_rotate_ranges[] = { {94, 102}, {112, 119}, {145, 153}, {167, 173}, {187, 192}, {200, 204}, {214, 221}, {234, 240}, {283, 291}, {303, 309},
													{324, 330}, {344, 350}, {362, 367}, {377, 382}, {405, 410}, {421, 439}, {445, 452}, {460, 465} };

class Activity_Timeline {
public:
	Activity_Timeline() = default;
	Activity_Timeline(std::string name, size_t size);
	Activity_Timeline(std::string name, size_t size, const std::pair<int, int>* begin, const std::pair<int, int>* end);
	Activity_Timeline(std::string name, std::vector<bool> t);
	Activity_Timeline(const Activity_Timeline& t1, const Activity_Timeline& t2);

	void fill_ranges(const std::pair<int, int>* begin, const std::pair<int, int>* end);
	void operator>>(size_t steps);

	float segment_accuracy(const Activity_Timeline& target);
	float bit_accuracy(const Activity_Timeline& target);
	float time_accuracy(const Activity_Timeline& target);

	void filter(const Activity_Timeline& target);

	void post_process(size_t consecutive_bit_length);
	void smooth_over(size_t max = 1);
	void compress(size_t factor = 10);
	void scale(size_t factor = 10);

	size_t get_total_time() const;
	void print(size_t rowsize = 10) const;
	size_t size() const;

public:
	std::string activity_name;
	std::vector<bool> timeline;
};

