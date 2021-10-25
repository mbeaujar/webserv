#include "Request.hpp"

Request::Request() :
    _host(),
    _method(),
    _path(),
    _content_length(),
    _content_type(),
	_transfer_encoding(),
	_error(200, "200 OK") {}

Request::Request(Request const & src)
	: _host(src._host),
	  _method(src._method),
	  _path(src._path),
      _content_length(src._content_length),
      _content_type(src._content_type),
	  _transfer_encoding(src._transfer_encoding),
	  _error(src._error) {}

Request::~Request() {}

Request & Request::operator=(Request const & rhs) {
    if (this != &rhs)
    {
        this->_host           = rhs._host;
        this->_method         = rhs._method;
        this->_path           = rhs._path;
        this->_content_length = rhs._content_length;
        this->_content_type   = rhs._content_type;
		this->_transfer_encoding = rhs._transfer_encoding;
		this->_error = rhs._error;
    }
    return *this;
}

void        Request::set_method(int const & method) { _method = method; }
void        Request::set_path(std::string const & path) { _path = path; }
void        Request::set_host(std::string const & host) { _host = host; }
void        Request::set_content_length(int len) { _content_length = len; }
void        Request::set_content_type(std::string const & type) { _content_type = type; }
void		Request::set_transfer_encoding(int const & t_p) { _transfer_encoding = t_p; }
void        Request::set_error(std::pair<int, std::string> const & error) { _error = error; }

std::string Request::get_host() const { return _host; }
std::string Request::get_path() const { return _path; }
int         Request::get_method() const { return _method; }
std::string Request::get_content_type() const { return _content_type; }
int         Request::get_content_length() const { return _content_length; }
int			Request::get_transfer_encoding() const { return _transfer_encoding; }
std::pair<int, std::string> Request::get_error() const { return _error; }
