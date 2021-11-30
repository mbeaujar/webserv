#include "Request.hpp"

Request::Request() :
    _host(),
    _method(),
    _path(),
	_query_string(),
    _content_length(),
    _content_type(),
	_transfer_encoding(),
	_error(200, "OK"),
	_return(-1, ""),
	_methods(),
	_date() {}

Request::Request(Request const & src)
	: _host(src._host),
	  _method(src._method),
	  _path(src._path),
	  _query_string(src._query_string),
      _content_length(src._content_length),
      _content_type(src._content_type),
	  _transfer_encoding(src._transfer_encoding),
	  _error(src._error),
	  _return(src._return),
	  _methods(),
	  _date(src._date) {}

Request::~Request() {}

Request & Request::operator=(Request const & rhs) {
    if (this != &rhs)
    {
        this->_host             	= rhs._host;
        this->_method           	= rhs._method;
        this->_path             	= rhs._path;
		this->_query_string			= rhs._query_string;
        this->_content_length   	= rhs._content_length;
        this->_content_type     	= rhs._content_type;
		this->_transfer_encoding	= rhs._transfer_encoding;
		this->_error            	= rhs._error;
		this->_return           	= rhs._return;
		this->_methods				= rhs._methods;
		this->_date             	= rhs._date;
    }
    return *this;
}

void        Request::set_method(int const & method) { _method = method; }
void        Request::set_path(std::string const & path) { _path = path; }
void        Request::set_query_string(std::string const & query_string) { _query_string = query_string; }
void        Request::set_host(std::string const & host) { _host = host; }
void        Request::set_content_length(int len) { _content_length = len; }
void        Request::set_content_type(std::string const & type) { _content_type = type; }
void		Request::add_transfer_encoding(std::string const & transfer_encoding) { _transfer_encoding.push_back(transfer_encoding); }
void        Request::set_error(std::pair<int, std::string> const & error) { _error = error; }
void        Request::set_return(std::pair<int, std::string> const & r) { _return = r; }
void        Request::set_methods(struct s_method & met) { _methods = met; }
void        Request::set_date(std::string const & date) { _date = date; }

std::string Request::get_host() const { return _host; }
std::string Request::get_path() const { return _path; }
std::string Request::get_query_string() const { return _query_string; }
int         Request::get_method() const { return _method; }
std::string Request::get_content_type() const { return _content_type; }
int         Request::get_content_length() const { return _content_length; }
std::vector<std::string>		Request::get_transfer_encoding() const { return _transfer_encoding; }
std::pair<int, std::string> Request::get_error() const { return _error; }
std::pair<int, std::string> Request::get_return() const { return _return; }
struct s_method     Request::get_methods() const { return _methods; }
std::string Request::get_date() const { return _date; }
