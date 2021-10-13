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

#include "server/Server.hpp"
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
    	throw std::invalid_argument("Wrong file 1");
    while (std::getline(file, line))
        content += line + "\n";
	if (content == "")
		throw std::invalid_argument("Wrong file 2");
    return content;
}

std::vector<Server> parser(const char *filename)
{
	std::vector<Server> servers;
	// Server *a = new Server();

	// servers.push_back(a);

	std::string file = readFile(filename);
	
	//servers = find_server(file);

	Server a = Server();
	a.cut_location(file);
	printVectorLocation(a.get_location());
	// servers.push_back(Server());
	//servers[0].cut_location(file);
	//printVectorLocation(servers[0].get_location());
	// check entre les serveur
	// if (!clean_file(file))

	// for (int i = 0; file[i]; i++)
	// {
		// if (find server)
			// servers.push_back(create_server());
	// }

	return servers;
}

