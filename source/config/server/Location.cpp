#include "Location.hpp"

/**
 * @brief Construct a new Location::Location object
 * 
 */
Location::Location() :
	_autoindex(false),
	_max_body(0),
	_root("/var/www/html"),
	_index(),
	_method(),
	_path_cgi(),
	_cgi_ext(),
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
Location::Location(Location const &copy) :
	_autoindex(copy._autoindex),
	_max_body(copy._max_body),
	_root(copy._root),
	_index(copy._index),
	_method(copy._method),
	_path_cgi(copy._path_cgi),
	_cgi_ext(copy._cgi_ext),
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
	_max_body = copy._max_body;
	_root = copy._root;
	_index = copy._index;
	_method = copy._method;
	_path_cgi = copy._path_cgi;
	_cgi_ext = copy._cgi_ext;
	_return = copy._return;
	return *this;
}

bool	Location::get_method(int nb) {
	if (nb == DELETE) {
		if (_method.m_delete == true)
			return true;
		return false;
	} else if (nb == GET) {
		if (_method.m_get == true)
			return true;
		return false;
	} else if (nb == POST) {
		if (_method.m_post == true)
			return true;
		return false;
	} 
	return false;
}

s_method& Location::get_methods() { return _method; }

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

void 							Location::set_upload(std::string const & upload) { _upload = upload; }
void 							Location::set_path_cgi(std::string const & path_cgi) { _path_cgi = path_cgi; }
void 							Location::set_cgi_ext(std::string const & cgi_ext) { _cgi_ext = cgi_ext; }
void 							Location::set_max_body(int const & max_body) { _max_body = max_body; }
int& 							Location::get_max_body() { return _max_body; }
bool& 							Location::get_autoindex() { return _autoindex; }
std::string& 					Location::get_upload() { return _upload; }
std::string& 					Location::get_path_cgi() { return _path_cgi; }
std::string& 					Location::get_cgi_ext() { return _cgi_ext; }
std::string& 					Location::get_root() { return _root; }
std::vector<std::string>&		Location::get_index() { return _index; }
std::pair<int, std::string>&	Location::get_return() { return _return; }

bool Location::find_index(std::string const & index) {
	std::vector<std::string>::const_iterator it = _index.begin(), ite = _index.end();

	while (it != ite) {
		if (*it == index)
			return true;
		++it;
	}
	return false;
}
