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
Location::Location(std::string location_block)
	:location_block(location_block),
	 autoindex(false),
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
	: location_block(copy.location_block),
		autoindex(copy.autoindex),
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
