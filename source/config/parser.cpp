// #include "server/Server.hpp"
#include "../prototype.hpp"
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief read the file in argument
 * 
 * @param filename 
 * @return std::string content of the file
 */
std::string read_file(std::string filename) 
{
    std::ifstream	file;
	std::ostringstream contents;

    file.open(filename.c_str());
    if (!file.is_open())
    	throw std::invalid_argument("can't open the file in argument");
    contents << file.rdbuf();
    file.close();
	if (contents.str() == "")
		throw std::invalid_argument("Empty file or file is a directory");
    return contents.str();
}


/**
 * @brief parse the file in argument 
 * 
 * @param filename path of the file
 * @return std::vector<Server> list of Server
 */
std::vector<Server> parser(const char *filename)
{
	std::vector<Server> servers;
	std::string file = read_file(filename);
	servers = parse_server(file);
	return servers;
}

