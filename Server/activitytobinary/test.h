// SIMS Group Project - Activity Tracker for Heavy Duty Machine, Project based product development - DT169G 
// Mathias Hammarström
// test.h, 10-10-2022, 31-10-2022
// Contains data from our tests at technichus.

#ifndef TEST_H
#define TEST_H
#include "Activity_Timeline.h"
#include "File_Parser.h"
#include <vector>
#include <algorithm>

const std::pair<int, int> test1_node3_ranges[] = { {37, 53}, {77, 78}, {81, 82}, {84, 85}, {94, 98}, {101, 105}, {115, 117}, {120, 121}, {124, 125},
													{131, 132}, {182, 183}, {186, 197}, {201, 207}, {209, 213}, {225, 226}, {230, 231}, {233, 234}, {238, 239},
													{242, 245}, {250, 251}, {254, 257}, {262, 263}, {272, 274}, {278, 284}, {289, 290}, {293, 295}, {302, 304} };
const std::pair<int, int> test1_node4_ranges[] = { {24, 37}, {83, 86}, {93, 97}, {102, 104}, {114, 115}, {123, 125}, {173, 180}, {211, 213}, {219, 223}, {234, 237},
													{245, 246}, {264, 268}, {273, 275}, {307, 310} };

const std::pair<int, int> test1_idle_ranges[]	= { {0, 23}, {131, 173}, {307, 335} };
const std::pair<int, int> test1_dig_ranges[]	= { {23, 54}, {76, 87}, {92, 106}, {113, 127}, {173, 215}, {220, 227}, {232, 248}, {262, 277}, {288, 297} };
const std::pair<int, int> test1_rotate_ranges[] = { {54, 76}, {87, 92}, {106, 113}, {127, 131}, {215, 220}, {227, 232}, {248, 262}, {277, 288}, {297, 307} };

const std::pair<int, int> test2_idle_ranges[]	= { {0, 94}, {240, 263}, {382, 401} };
const std::pair<int, int> test2_dig_ranges[]	= { {102, 112}, {119, 145}, {153, 167}, {173, 187}, {192, 200}, {204, 214}, {221, 234}, {263, 283}, {291, 303}, {309, 324},
													{330, 344}, {350, 362}, {367, 377}, {401, 405}, {410, 421}, {439, 445}, {452, 460} };
const std::pair<int, int> test2_rotate_ranges[] = { {94, 102}, {112, 119}, {145, 153}, {167, 173}, {187, 192}, {200, 204}, {214, 221}, {234, 240}, {283, 291}, {303, 309},
													{324, 330}, {344, 350}, {362, 367}, {377, 382}, {405, 410}, {421, 439}, {445, 452}, {460, 465} };

const std::pair<int, int> test2_node3_ranges[]	= { {94, 95}, {100, 103}, {108, 111}, {113, 115}, {120, 123}, {133, 135}, {141, 143}, {153, 159}, {163, 165}, {176, 180},
													{185, 185}, {188, 188}, {192, 192}, {194, 198}, {204, 205}, {222, 226}, {230, 230}, {265, 275}, {277, 280}, {291, 293},
													{300, 300}, {309, 316}, {318, 320}, {322, 323}, {332, 334}, {341, 341}, {351, 352}, {355, 358}, {382, 384}, {401, 404},
													{414, 415}, {417, 419}, {423, 423}, {430, 430}, {439, 440}, {443, 444}, {452, 453}, {458, 459} };

const std::pair<int, int> test2_node4_ranges[]	= { {130, 132}, {142, 144}, {155, 157}, {172, 175}, {178, 180}, {193, 197}, {221, 222}, {224, 226}, {231, 233}, {263, 264},
													{267, 268}, {272, 273}, {287, 290}, {293, 297}, {301, 303}, {306, 307}, {311, 312}, {321, 322}, {330, 331}, {333, 337},
													{342, 344}, {349, 350}, {367, 370}, {375, 377}, {401, 403}, {410, 414}, {424, 425}, {431, 432} };

namespace Test {
	// Our manually labeled timelines
	Activity_Timeline idle1_timeline("Idle test 1", 335, &test1_idle_ranges[0], &test1_idle_ranges[3]);
	Activity_Timeline dig1_timeline("Dig test 1", 335, &test1_dig_ranges[0], &test1_dig_ranges[9]);
	Activity_Timeline rotate1_timeline("Rotate test 1", 335, &test1_rotate_ranges[0], &test1_rotate_ranges[9]);
	Activity_Timeline node3_piston1_timeline("Node3 piston test 1", 335, &test1_node3_ranges[0], &test1_node3_ranges[27]);
	Activity_Timeline node4_piston1_timeline("Node4 piston test 1", 335, &test1_node4_ranges[0], &test1_node4_ranges[14]);

	Activity_Timeline idle2_timeline("Idle test 2", 465, &test2_idle_ranges[0], &test2_idle_ranges[3]);
	Activity_Timeline dig2_timeline("Dig test 2", 465, &test2_dig_ranges[0], &test2_dig_ranges[17]);
	Activity_Timeline rotate2_timeline("Rotate test 2", 465, &test2_rotate_ranges[0], &test2_rotate_ranges[18]);
	Activity_Timeline node3_piston2_timeline("Node3 piston test 2", 465, &test2_node3_ranges[0], &test2_node3_ranges[38]);
	Activity_Timeline node4_piston2_timeline("Node4 piston test 2", 465, &test2_node4_ranges[0], &test2_node4_ranges[28]);

	std::vector<float> test1_node3_ultrasonic;
	std::vector<float> test1_node4_ultrasonic;
	std::vector<float> test1_node4_lidar;
	std::vector<float> test1_node4_acc_x;
	std::vector<float> test1_node4_acc_y;
	std::vector<float> test1_node4_acc_z;
	std::vector<float> test1_node4_gyro_x;
	std::vector<float> test1_node4_gyro_y;
	std::vector<float> test1_node4_gyro_z;
	
	std::vector<float> test2_node3_ultrasonic;
	std::vector<float> test2_node4_ultrasonic;
	std::vector<float> test2_node4_lidar;
	std::vector<float> test2_node4_acc_x;
	std::vector<float> test2_node4_acc_y;
	std::vector<float> test2_node4_acc_z;
	std::vector<float> test2_node4_gyro_x;
	std::vector<float> test2_node4_gyro_y;
	std::vector<float> test2_node4_gyro_z;

	std::vector<float> test2_master_gyro_x;
	std::vector<float> test2_master_gyro_y;
	std::vector<float> test2_master_gyro_z;

	void shift_arr(std::vector<float>& vec, int step) {
		for (int i = 0; i < step; ++i) {
			vec.push_back(0.0);
		}
		std::rotate(vec.begin(), vec.end() - step, vec.end());
	}

	void initTestData() {
		copy_file_to_array("data/Test1_Node3_Sonic.log",	test1_node3_ultrasonic);
		copy_file_to_array("data/Test1_Node4_Sonic.log",	test1_node4_ultrasonic);
		copy_file_to_array("data/Test1_Node4_Lidar.log",	test1_node4_lidar);
		copy_file_to_array("data/Test1_Node4_AccX.log",		test1_node4_acc_x);
		copy_file_to_array("data/Test1_Node4_AccY.log",		test1_node4_acc_y);
		copy_file_to_array("data/Test1_Node4_AccZ.log",		test1_node4_acc_z);
		copy_file_to_array("data/Test1_Node4_GyroX.log",	test1_node4_gyro_x);
		copy_file_to_array("data/Test1_Node4_GyroY.log",	test1_node4_gyro_y);
		copy_file_to_array("data/Test1_Node4_GyroZ.log",	test1_node4_gyro_z);

		copy_file_to_array("data/Test2_Node3_Sonic.log",	test2_node3_ultrasonic);
		copy_file_to_array("data/Test2_Node4_Sonic.log",	test2_node4_ultrasonic);
		copy_file_to_array("data/Test2_Node4_Lidar.log",	test2_node4_lidar);
		copy_file_to_array("data/Test2_Node4_AccX.log",		test2_node4_acc_x);
		copy_file_to_array("data/Test2_Node4_AccY.log",		test2_node4_acc_y);
		copy_file_to_array("data/Test2_Node4_AccZ.log",		test2_node4_acc_z);
		copy_file_to_array("data/Test2_Node4_GyroX.log",	test2_node4_gyro_x);
		copy_file_to_array("data/Test2_Node4_GyroY.log",	test2_node4_gyro_y);
		copy_file_to_array("data/Test2_Node4_GyroZ.log",	test2_node4_gyro_z);
		copy_file_to_array("data/Test2_Master_GyroX.log",	test2_master_gyro_x);
		copy_file_to_array("data/Test2_Master_GyroY.log",	test2_master_gyro_y);
		copy_file_to_array("data/Test2_Master_GyroZ.log",	test2_master_gyro_z);

		test1_node3_ultrasonic = derivative(test1_node3_ultrasonic);
		test1_node4_ultrasonic = derivative(test1_node4_ultrasonic);
		test1_node4_lidar = derivative(test1_node4_lidar);
		test1_node4_acc_x = derivative(test1_node4_acc_x);
		test1_node4_acc_y = derivative(test1_node4_acc_y);
		test1_node4_acc_z = derivative(test1_node4_acc_z);

		// We shift the data to correct for offset between video and sensor node data when we were testing
		shift_arr(test1_node3_ultrasonic, 70);
		shift_arr(test1_node4_ultrasonic, 98);
		shift_arr(test1_node4_lidar, 98);
		shift_arr(test1_node4_acc_x, 98);
		shift_arr(test1_node4_acc_y, 98);
		shift_arr(test1_node4_acc_z, 98);
		shift_arr(test1_node4_gyro_x, 98);
		shift_arr(test1_node4_gyro_y, 98);
		shift_arr(test1_node4_gyro_z, 98);


		test2_node3_ultrasonic = derivative(test2_node3_ultrasonic);
		test2_node4_ultrasonic = derivative(test2_node4_ultrasonic);
		test2_node4_lidar = derivative(test2_node4_lidar);
		test2_node4_acc_x = derivative(test2_node4_acc_x);
		test2_node4_acc_y = derivative(test2_node4_acc_y);
		test2_node4_acc_z = derivative(test2_node4_acc_z);

		for (int i = 0; i < test2_master_gyro_x.size(); ++i) {
			if (abs(test2_master_gyro_x[i]) > 5) {
				test2_master_gyro_x[i] = test2_master_gyro_x[i - 1];
			}
			if (abs(test2_master_gyro_y[i]) > 5) {
				test2_master_gyro_y[i] = test2_master_gyro_y[i - 1];
			}
			if (abs(test2_master_gyro_z[i]) > 5) {
				test2_master_gyro_z[i] = test2_master_gyro_z[i - 1];
			}
		}
		// We shift the data to correct for offset between video and sensor node data when we were testing
		shift_arr(test2_node3_ultrasonic, 687);
		shift_arr(test2_node4_ultrasonic, 210);
		shift_arr(test2_node4_lidar, 210);
		shift_arr(test2_node4_acc_x, 210);
		shift_arr(test2_node4_acc_y, 210);
		shift_arr(test2_node4_acc_z, 210);
		shift_arr(test2_node4_gyro_x, 210);
		shift_arr(test2_node4_gyro_y, 210);
		shift_arr(test2_node4_gyro_z, 210);
		shift_arr(test2_master_gyro_x, 190);
		shift_arr(test2_master_gyro_y, 190);
		shift_arr(test2_master_gyro_z, 190);
	}
	
}







#endif
