#ifndef __PROTOTYPE_HPP__
#define __PROTOTYPE_HPP__

#include "parsing/server/Server.hpp"
#include <vector>

std::vector<Server>	parser(const char *filename);
std::vector<Server>	find_server(std::string file);
Server				create_server(std::string server);
int					parse_error_page(std::string file, int i, Server &a);
int					parse_elements(std::string file, int i, Server &a);
int					parse_root(std::string file, int i, Server &a);
int					skip_bracket(std::string file, int i);
int					skip_comment(std::string file, int i);
int					skip_space(std::string file, int i);
int					skip_word(std::string file, int i);
int 				parse_listen(std::string file, int i, Server &a);

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