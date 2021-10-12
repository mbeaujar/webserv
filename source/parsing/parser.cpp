/**
 * @file parser.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Server.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include "../prototype.hpp"


std::string readFile(std::string filename) 
{
    std::string	    content, line;
    std::ifstream	file;

    file.open(filename.c_str());
    if (!file.is_open())
    	throw std::invalid_argument("Wrong file");
    while (std::getline(file, line))
        content += line + "\n";
	if (content == "")
		throw std::invalid_argument("Wrong file");
    return content;
}

std::vector<Server> parser(const char *filename)
{
	std::vector<Server> servers;
	// Server *a = new Server();

	// servers.push_back(a);

	std::string file = readFile(filename);
	Location location1;
	location1.cut_location(file);
	
	// check entre les serveur
	// if (!clean_file(file))

	// for (int i = 0; file[i]; i++)
	// {
		// if (find server)
			// servers.push_back(create_server());
	// }

	return servers;
}

