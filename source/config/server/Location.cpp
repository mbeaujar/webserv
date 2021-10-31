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
	  _fastcgi(),
	  _fastcgi_ext(),
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
	  _fastcgi(copy._fastcgi),
	  _fastcgi_ext(copy._fastcgi_ext),
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
	_fastcgi = copy._fastcgi;
	_fastcgi_ext = copy._fastcgi_ext;
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

void Location::set_fastcgi(std::string const & fastcgi) { _fastcgi = fastcgi; }

void Location::set_fastcgi_ext(std::string const & fastcgi_ext) { _fastcgi_ext = fastcgi_ext; }

bool Location::get_autoindex() const { return _autoindex; }

std::string Location::get_fastcgi() const { return _fastcgi; }

std::string Location::get_fastcgi_ext() const { return _fastcgi_ext; }

std::string Location::get_root() const { return _root; }

std::vector<std::string> Location::get_index() const { return _index; }

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
