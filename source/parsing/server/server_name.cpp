/**
 * @file server_name.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include "Server.hpp"
#include "../../prototype.hpp"

int skip_word(std::string file, int i) {
	while (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '\n' && file[i] != '#')
		i++;
	return i;
}

int	parse_elements(std::string file, int i, Server &a)
{
	int server_name = 0;
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && file[i] != ';' && file[i] != '#') {
			a.add_Server_name(file.substr(i, skip_word(file, i)));
			i = skip_word(file, i);
			server_name++;
		}
	}
	if (file[i] != ';')
		throw std::invalid_argument("Wrong file 6");
	if (server_name == 0)
		throw std::invalid_argument("Wrong file 10");
	return i;
}

int	parse_root(std::string file, int i, Server &a)
{
	int root = 0;
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && file[i] != ';' && file[i] != '#') {
			if (root == 1)
				throw std::invalid_argument("Wrong file 12");
			a.add_Server_name(file.substr(i, skip_word(file, i)));
			i = skip_word(file, i);
			root++;
		}
	}
	if (file[i] != ';')
		throw std::invalid_argument("Wrong file 13");
	return i;
}

int	parse_error_page(std::string file, int i, Server &a)
{
	int counter = 0;
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && file[i] != ';' && file[i] != '#') {
			if (root == 1)
				throw std::invalid_argument("Wrong file 12");
			a.add_Server_name(file.substr(i, skip_word(file, i)));
			i = skip_word(file, i);
			root++;
		}
	}
	if (file[i] != ';')
		throw std::invalid_argument("Wrong file 13");
	return i;
}









