/**
 * @file main.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "prototype.hpp"
#include <vector>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "number of arguments invalid" << "\n";
		return 1;
	}
	std::vector<Server> servers;
	// parsing 
	try {
		servers = parser(argv[1]);
	}
    catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	// socket


	return 0;
}

