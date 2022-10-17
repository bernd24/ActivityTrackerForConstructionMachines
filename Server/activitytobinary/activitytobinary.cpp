// activitytobinary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "Activity_Timeline.h"
#include "Data_Analysis.h"
#include "File_Parser.h"

int main()
{
	Activity_Timeline dig_timeline("Digging", 335, &test1_dig_ranges[0], &test1_dig_ranges[9]);
	Activity_Timeline rotate_timeline("Rotating", 335, &test1_rotate_ranges[0], &test1_rotate_ranges[9]);
	Activity_Timeline idle_timeline("Idle", 335, &test1_idle_ranges[0], &test1_idle_ranges[3]);

	Activity_Timeline dig2_timeline("Digging test2", 465, &test2_dig_ranges[0], &test2_dig_ranges[17]);
	Activity_Timeline rotate2_timeline("Rotating 2", 465, &test2_rotate_ranges[0], &test2_rotate_ranges[18]);

	Activity_Timeline node3_piston("Node3", 335, &test1_node3_ranges[0], &test1_node3_ranges[27]);
	Activity_Timeline node4_piston("Node4", 335, &test1_node4_ranges[0], &test1_node4_ranges[14]);
	
	std::vector<float> node4_ultrasonic;
	std::vector<float> node4_lidar;
	std::vector<float> node3_ultrasonic;

	std::vector<float> test2_node4_sonic;
	std::vector<float> test2_node3_sonic;
	std::vector<float> node4_acc_x;
	std::vector<float> node4_acc_y;
	std::vector<float> node4_acc_z;
	std::vector<float> node4_gyro_x;
	std::vector<float> node4_gyro_y;
	std::vector<float> node4_gyro_z;

	std::vector<float> test2_master_gyro_x;
	std::vector<float> test2_master_gyro_y;
	std::vector<float> test2_master_gyro_z;

	// About 8 second offset
	copy_file_to_array("Test1_Node4_Sonic.log", node4_ultrasonic);
	copy_file_to_array("Test1_Node4_Lidar.log", node4_lidar);
	copy_file_to_array("Test1_Node4_AccX.log", node4_acc_x);
	copy_file_to_array("Test1_Node4_AccY.log", node4_acc_y);
	copy_file_to_array("Test1_Node4_AccZ.log", node4_acc_z);
	copy_file_to_array("Test1_Node4_GyroX.log", node4_gyro_x);
	copy_file_to_array("Test1_Node4_GyroY.log", node4_gyro_y);
	copy_file_to_array("Test1_Node4_GyroZ.log", node4_gyro_z);
	copy_file_to_array("Test2_Master_GyroX.log", test2_master_gyro_x);
	copy_file_to_array("Test2_Master_GyroY.log", test2_master_gyro_y);
	copy_file_to_array("Test2_Master_GyroZ.log", test2_master_gyro_z);

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
	// About 7 second offset
	copy_file_to_array("Test1_Node3_Sonic.log", node3_ultrasonic);
	
	copy_file_to_array("Test2_Node4_Sonic.log", test2_node4_sonic);
	copy_file_to_array("Test2_Node3_Sonic.log", test2_node3_sonic);

	
	// Node 3 testing for test1
	//get_best_format_bruteforce(node4_gyro_z, rotate_timeline).print();
	int perm[] = { 0, 1, 2, 3};
	std::vector<float> node3_d = derivative(node3_ultrasonic);
	std::vector<float> node4_d = derivative(node4_ultrasonic);
	std::vector<float> node34_d;
	
	for (int i = 0; i < 30; ++i) {
		node34_d.push_back(node3_d[i] / 2);
	}

	int i = 0;
	while(i < node3_d.size() - 30 && i < node4_d.size()) {
		node34_d.push_back((node3_d[i + 30] + node4_d[i]) / 2);
		++i;
	}
	while (i < node4_d.size()) {
		node34_d.push_back(node4_d[i] / 2);
	}
	std::vector<float> node4_accx = node4_gyro_x;// derivative(node4_gyro_x);
	std::vector<float> node4_accy = node4_gyro_y;// derivative(node4_gyro_y);
	std::vector<float> node4_accz = node4_gyro_z;// derivative(node4_gyro_z);

	
	std::vector<bool> t = { 1, 1, 1, 0, 0, 1 };
	Activity_Timeline node3 = convert_to_raw_binary("Predicted node3", node3_d, 1700, 1.0);
	Activity_Timeline node4 = convert_to_raw_binary("Predicted node4", node4_d, 1700, 1.0);
	size_t compress_factor = 10;
	size_t print_rowsize = 100;

	//node3.compress(compress_factor);
	//node4.compress(compress_factor);
	//node3_piston.compress(5);
	//node4_piston.compress(5);
	//dig_timeline.compress(5);

	node3_piston.scale(compress_factor);
	node4_piston.scale(compress_factor);
	dig_timeline.scale(compress_factor);

	node3.post_process(5);

	node3.print(print_rowsize);
	node3_piston.print(print_rowsize);
	std::cout << "Node 3 Acc: " << node3.bit_accuracy(node3_piston) << "\n";
	std::cout << "Time acc: " << node3.time_accuracy(node3_piston) << "\n";

	
	node4.print(print_rowsize);
	node4_piston.print(print_rowsize);
	std::cout << "Node 4 Acc: " << node4.bit_accuracy(node4_piston) << "\n";
	std::cout << "Time acc: " << node4.time_accuracy(node4_piston) << "\n";

	Activity_Timeline dig(node3, node4);
	
	dig.print(print_rowsize);
	dig_timeline.print(print_rowsize);
	std::cout << "Dig Acc: " << dig.bit_accuracy(dig_timeline) << "\n";
	std::cout << "Time acc: " << dig.time_accuracy(dig_timeline) << "\n";

	Activity_Timeline empty("empty", 465);
	std::cout << "Reference Acc: " << empty.bit_accuracy(rotate2_timeline) << "\n";

}