#ifndef __PROTOTYPE_HPP__
#define __PROTOTYPE_HPP__

#include "config/server/Server.hpp"
#include "config/server/Location.hpp"
#include <vector>
#include <sstream>

std::vector<Server>	parser(const char *filename);
std::vector<Server> parse_server(std::string file);
Server 				config_server(std::string file);
Location 			config_location(std::string file);
int					parse_error_page(std::string file, int i, Server &a);
int					parse_elements(std::string file, int i, Server &a);
int					parse_root(std::string file, int i, Server &a);
int					skip_bracket(std::string file, int i);
int					skip_comment(std::string file, int i);
int					skip_space(std::string file, int i);
int					skip_word(std::string file, int i);
int 				skip_word_exception(std::string file, int i);
int 				parse_listen(std::string file, int i, Server &a);
int					parse_error_page(std::string file, int i, Server &a);
int 				parse_client_size(std::string file, int i, Server &a);
int					recup_nb(std::string file, int & i);
int					parse_autoindex(std::string file, int i, Location &a);
int 				parse_index(std::string file, int i, Location &a);
int 				parse_root(std::string file, int i, Location &a);

template <typename tostring>
std::string to_string(tostring n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

// template <typename T>
// void printVector(T &vector)
// {
// 	typename T::iterator iter;

// 	std::cout << "vector: ";
// 	iter = vector.begin();
//     std::cout << '[';
// 	while (iter != vector.end())
// 	{
// 		std::cout << *iter;
// 		if (iter + 1 != vector.end())
// 			std::cout << ",";
// 		iter++;
// 	}
//     std::cout << ']';
// 	std::cout << std::endl;
// }

// template <typename T>
// void printVectorLocation(T vector)
// {
// 	typename T::iterator iter;

// 	std::cout << "Location : ";
// 	iter = vector.begin();
//     std::cout << '[';
// 	while (iter != vector.end())
// 	{
// 		std::cout << iter->location_block;
//         std::cout << std::endl;
// 		if (iter + 1 != vector.end())
// 			std::cout << "***************************" << std::endl;
// 		iter++;
// 	}
//     std::cout << ']';
// 	std::cout << std::endl;
// }

#endif