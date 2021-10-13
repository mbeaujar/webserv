#include "Location.hpp"

/**
 * @brief Construct a new Location::Location object
 * 
 */
Location::Location()
	: _autoindex(false),
	  _index(),
	  _fastcgi_param(),
	  _fastcgi_pass() {}

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
	: _autoindex(copy._autoindex),
	  _index(copy._index),
	  _fastcgi_param(copy._fastcgi_param),
	  _fastcgi_pass(copy._fastcgi_pass) {}

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
	_autoindex = copy._autoindex;
	_index = copy._index;
	_fastcgi_param = copy._fastcgi_param;
	_fastcgi_pass = copy._fastcgi_pass;
	return *this;
}
