#include "Request.hpp"
#include "../socket.hpp"

Request::Request() :
    _method(),
    _content_length(-1),
    _host(),
    _path(),
	_file(),
	_date(),
	_query_string(),
    _content_type(),
	_methods(),
	_error(200, "OK"),
	_return(-1, "") {}

Request::Request(Request const & src) :
  	_method(src._method),
    _content_length(src._content_length),
	_host(src._host),
	_path(src._path),
	_file(src._file),
	_date(src._date),
	_query_string(src._query_string),
    _content_type(src._content_type),
	_methods(),
	_error(src._error),
	_return(src._return) {}

Request::~Request() {}

Request & Request::operator=(Request const & rhs) {
    if (this != &rhs)
    {
        this->_method           	= rhs._method;
        this->_content_length   	= rhs._content_length;
        this->_host             	= rhs._host;
        this->_path             	= rhs._path;
		this->_file 				= rhs._file;
		this->_date             	= rhs._date;
		this->_query_string			= rhs._query_string;
        this->_content_type     	= rhs._content_type;
		this->_methods				= rhs._methods;
		this->_error            	= rhs._error;
		this->_return           	= rhs._return;
    }
    return *this;
}


void Request::set_file(std::string & file)
{
	if (is_directory(file))
	{
		if (file.length() > 1 && file[file.length() - 1] != '/')
				file.push_back('/');
	}
	_file = file;
}
void        				Request::set_method(int const & method) { _method = method; }
void        				Request::set_path(std::string const & path) { _path = path; }
void        				Request::set_date(std::string const & date) { _date = date; }
void        				Request::set_host(std::string const & host) { _host = host; }
void        				Request::set_content_length(int & len) { _content_length = len; }
void        				Request::set_methods(struct s_method & met) { _methods = met; }
void        				Request::set_return(std::pair<int, std::string> const & r) { _return = r; }
void        				Request::set_content_type(std::string const & type) { _content_type = type; }
void        				Request::set_error(std::pair<int, std::string> const & error) { _error = error; }
void        				Request::set_query_string(std::string const & query_string){ _query_string = query_string; }

int &        				Request::get_method() { return _method; }
int &       				Request::get_content_length() { return _content_length; }
std::string &				Request::get_host() { return _host; }
std::string &				Request::get_date() { return _date; }
std::string &				Request::get_query_string() { return _query_string; }
std::string &				Request::get_content_type() { return _content_type; }
std::string & 				Request::get_path() { return _path; }
struct s_method	&			Request::get_methods() { return _methods; }
std::string const & 		Request::get_file() { return _file; }
std::pair<int, std::string> &Request::get_error() { return _error; }
std::pair<int, std::string> &Request::get_return() { return _return; }
