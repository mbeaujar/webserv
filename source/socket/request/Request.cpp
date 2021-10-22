#include "Request.hpp"

Request::Request() :
    _host(),
    _method(),
    _path() {}

Request::Request(Request const & src)
	: _host(src._host),
	  _method(src._method),
	  _path(src._path) {}

Request::~Request() {}

Request & Request::operator=(Request const & rhs) {
    if (this != &rhs)
    {
        this->_host   = rhs._host;
        this->_method = rhs._method;
        this->_path   = rhs._path;
    }
    return *this;
}

void Request::set_method(int const & method) { _method = method; }

void Request::set_path(std::string const & path) { _path = path; }

void Request::set_host(std::string const & host) { _host = host; }

std::string     Request::get_host() const { return _host; }

int             Request::get_method() const { return _method; }

std::string     Request::get_path() const { return _path; }