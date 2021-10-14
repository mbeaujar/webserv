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

void Location::adding_index(std::string const & index) {
	_index.push_back(index);
}

void Location::adding_param(std::string const & param) {
	_fastcgi_param.push_back(param);
}

void Location::adding_pass(std::string const & pass) {
	_fastcgi_pass.push_back(pass);
}

void Location::set_root(std::string const & root) {
	_root = root;
}

void Location::set_autoindex(bool const & autoindex) {
	_autoindex = autoindex;
}

bool Location::get_autoindex() const { return _autoindex; }

std::string Location::get_root() const { return _root; }

bool Location::find_index(std::string const & index) const {
	std::vector<std::string>::const_iterator it = _index.begin(), ite = _index.end();

	while (it != ite) {
		if (*it == index)
			return true;
		++it;
	}
	return false;
}

// bool Location::find_param(std::string const & param) const {

// }
