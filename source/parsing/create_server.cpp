/**
 * @file create_server.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include "Server.hpp"


void escape_scape(std::string file, int &i) {
	while (isspace(file[i]) && file[i] != '\n')
		i++;
}

// Server *create_server(std::string file) {
// 	Server *serv = new Server();
// 	int i = 0;

// 	while (file[i] != '{')
// 		i++;
// 	for (; file[i]; i++)
// 	{
		
// 	}
// }