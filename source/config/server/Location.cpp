#include "Location.hpp"

/**
 * @brief Construct a new Location::Location object
 * 
 */
Location::Location()
	: _autoindex(false),
	  _root("/var/www/html"),
	  _index(),
	  _method(),
	  _fastcgi_param(),
	  _fastcgi_pass(),
	  _return(-1, "") {}

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
	  _root(copy._root),
	  _index(copy._index),
	  _method(copy._method),
	  _fastcgi_param(copy._fastcgi_param),
	  _fastcgi_pass(copy._fastcgi_pass),
	  _return(copy._return) {}

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
	_root = copy._root;
	_index = copy._index;
	_method = copy._method;
	_fastcgi_param = copy._fastcgi_param;
	_fastcgi_pass = copy._fastcgi_pass;
	_return = copy._return;
	return *this;
}

bool Location::get_method(int nb) const
{
    if (nb == DELETE)
        return true;
    else if (nb == GET)
        return true;
    else
        return true;
}

s_method Location::get_methods() const { return _method; }

void Location::adding_method(int nb) {

    if (nb == DELETE)
        _method.m_delete = true;
    else if (nb == GET)
        _method.m_get = true;
    else
        _method.m_post = true;
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

void Location::set_return(int const & code, std::string const & path) {
	if (_return.first == -1)  {
		_return.first = code;
		_return.second = path;
	}
}

bool Location::get_autoindex() const { return _autoindex; }

std::string Location::get_root() const { return _root; }

std::vector<std::string> Location::get_index() const { return _index; }

std::vector<std::string> Location::get_param() const { return _fastcgi_param; }

std::vector<std::string> Location::get_pass() const { return _fastcgi_pass; }

std::pair<int, std::string> Location::get_return() const { return _return; }

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
