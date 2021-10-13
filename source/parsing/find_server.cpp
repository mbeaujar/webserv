/**
 * @file find_server.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include "server/Server.hpp"
#include "../prototype.hpp"


int skip_space(std::string file, int i) {
	while (file[i] && isspace(file[i]))
		i++;
	return i;
}

int skip_bracket(std::string file, int i) {
	int counter = 0;
	while (file[i]) {
		if (file[i] == '#')
			i = skip_comment(file, i);
		if (file[i] == '{')
			counter++;
		if (file[i] == '}')
			counter--;
		if (counter == 0)
			break;
		i++;
	}
	return i;
}

int skip_comment(std::string file, int i) {
	while (file[i] && file[i] != '\n')
		i++;
	if (file[i] == '\n')
		i++;
	return i;
}

std::vector<Server> find_server(std::string file)
{
	std::vector<Server> lst;

	for (int i = 0; file[i]; i++)
	{
		i = skip_space(file, i);
		if (!file[i])
			break;
		if (file.compare(i, 6, "server") == 0) 
		{
			i += 6;
			while (file[i] && file[i] != '{') { // skip space + comment
				if (isspace(file[i]))
					i = skip_space(file, i);
				else if (file[i] == '#')
					i = skip_comment(file, i);
				else 
					throw std::invalid_argument("Wrong file 5");
			}
			if (file[i] == '{') { // add a new server
				int skip = skip_bracket(file, i);
				std::string server = file.substr(i, skip);
				// lst.push_back(create_server(server));
				std::cout << "find" << std::endl;
				i = skip;
			}
			else
				throw std::invalid_argument("Wrong file 4");
		}
		else if (file[i] == '#')
			i = skip_comment(file, i);
		else if (!isspace(file[i]))
			throw std::invalid_argument("Wrong file 3");
	}
	return lst;
}