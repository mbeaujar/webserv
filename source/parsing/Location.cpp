/**
 * @file Location.cpp
 * @author mbeaujar (mbeaujar@student.42.fr)
 * @brief 
 * @version 0.1
 * @date 2021-10-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Location.hpp"

/**
 * @brief Construct a new Location::Location object
 * 
 */
Location::Location()
	: autoindex(false),
	  index(),
	  fastcgi_param(),
	  fastcgi_pass() {}

/**
 * @brief Destroy the Location::Location object
 * 
 */
Location::~Location() {}

/**
 * @brief Construct a new Location::Location object
 * 
 * @fastcgi_param copy 
 */
Location::Location(Location const &copy)
	: autoindex(copy.autoindex),
	  index(copy.index),
	  fastcgi_param(copy.fastcgi_param),
	  fastcgi_pass(copy.fastcgi_pass) {}

/**
 * @brief 
 * 
 * @fastcgi_param copy 
 * @return Location& 
 */
Location& Location::operator=(Location const &copy) 
{
	if (this == &copy)
		return *this;
	autoindex = copy.autoindex;
	index = copy.index;
	fastcgi_param = copy.fastcgi_param;
	fastcgi_pass = copy.fastcgi_pass;
	return *this;
}

void Location::cut_location(std::string file)
{
	int i = 0;
	int debut = 0;
	int fin = 0;

	while (file[i] != '\0')
	{
		if (file[i] == 'l' && 
		file[i + 1] && file[i + 1] == 'o' && 
		file[i + 2] && file[i + 2] == 'c' && 
		file[i + 3] && file[i + 3] == 'a' && 
		file[i + 4] && file[i + 4] == 't' && 
		file[i + 5] && file[i + 5] == 'i' && 
		file[i + 6] && file[i + 6] == 'o' && 
		file[i + 7] && file[i + 7] == 'n')
		{
			debut = i;
			i+=7;
			while (file[i] && file[i] != '}')
				i++;
			if (file[i] == '}')
				fin = i;
			else
				throw std::invalid_argument("Wrong file");
		}
		i++;
	}
	location_block = file.substr(debut, fin);
	std::cout << location_block << std::endl;
}