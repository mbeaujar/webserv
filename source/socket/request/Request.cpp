#include "Request.hpp"

Request::Request() :
    _host(),
    _method(),
    _path() {}

Request::Request(Request const & src) {
    this->_host   = src._host;
    this->_method = src._method;
    this->_path   = src._path;
}

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

void Request::set_method(int m) { _method = m; }

void Request::set_path(std::string p) { _path = p; }

void Request::set_host(std::string h) { _host = h; }