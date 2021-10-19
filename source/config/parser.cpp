// #include "server/Server.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include "../prototype.hpp"

/**
 * @brief read the file in argument
 * 
 * @param filename 
 * @return std::string content of the file
 */
std::string read_file(std::string filename) 
{
    std::string	    content, line;
    std::ifstream	file;

    file.open(filename.c_str());
    if (!file.is_open())
    	throw std::invalid_argument("can't open the file in argument");
    while (std::getline(file, line))
        content += line + "\n";
	if (content == "")
		throw std::invalid_argument("Empty file or file is a directory");
    return content;
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

