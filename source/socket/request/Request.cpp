#include "Request.hpp"
#include "../../config/server/Server.hpp"
#include "../proto.hpp"
#include <cstring>

Request::Request(int & client_socket, Server const & server) :
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
	_return(-1, "")
{
	int len = 0;
	char *header = this->read_header(client_socket);

	if (header)
		len = strlen(header);
	
	if (header == NULL)
	{
		std::cerr << "webserv: [warn]: class Request: Constructor: can't read header request" << std::endl;
		this->set_error(std::make_pair(500, "Internal Server Error"));
	}
	else if (header && len == 0)
	{
		std::cerr << "webserv: [warn]: class Request: Constructor: empty header in the request" << std::endl;
		this->set_error(std::make_pair(400, "Bad Request"));
	}
	else if (len > MAX_LEN - 1)
	{
		std::cerr << "webserv: [warn]: class Request: Constructor: header in the request too large" << std::endl;
		this->set_error(std::make_pair(413, "Request Entity Too Large"));
	}
	else 
		parse_header(header, server);
	delete [] header;
}

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

Request & Request::operator=(Request const & rhs)
{
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
std::string  & 				Request::get_file() { return _file; }
std::pair<int, std::string> &Request::get_error() { return _error; }
std::pair<int, std::string> &Request::get_return() { return _return; }



int Request::find_query_string(std::string & request, int i)
{
	while (request[i] && request[i] != '?')
		++i;
	return i;
}

void lower_file(std::string & request)
{
	int i = 0;
	while (request[i])
	{
		if (request[i] >= 'A' && request[i] <= 'Z')
			request[i] += 32;
		if (request[i] == ':' || request[i] == ' ')
		{
			while (request[i] && request[i] != '\n')
				++i;
		}
		if (request[i])
			++i;
	}
}

int Request::skip_the_word(std::string & file, int i) {
	while (file[i] && !isspace(file[i]))
		++i;
	return i;
}

std::string Request::recup_word(std::string & request, int & i)
{
	int tmp;
	std::string word;

	tmp = skip_the_word(request, i);
	word = request.substr(i, tmp - i);
	i = skip_space(request, tmp);
	return word;
}

void	Request::get_first_line(std::string & request, Server const & server)
{
	int i = 0;
	std::string word;
	(void)server;

	word = recup_word(request, i);
	if (word == "delete")
		this->set_method(DELETE);
	else if (word == "post")
		this->set_method(POST);
	else if (word == "get")
		this->set_method(GET);
	// else
	// {
	// 	int j = i;
	// 	std::pair<Location, std::string> a = search_location(recup_word(request, j), server);
	// 	s_method tmp = a.first.get_methods();
	// 	this->set_methods(tmp);
	// 	this->set_error(std::make_pair(405, "Method not allowed"));
	// }
	word = recup_word(request, i);
	if (word.length() > 1 && word[word.length() - 1] == '/')
		word.erase(--word.end());
	this->set_path(word);
	word = recup_word(request, i);
	if (word != "HTTP/1.1")
	{
		std::cerr << "webserv: [warn]: get_first_line: Bad HTTP version: " << word << std::endl;
		this->set_error(std::make_pair(505, "HTTP Version Not Supporteds"));
	}
	word = recup_word(request, i);
	if (word != "host:")
	{
		std::cerr << "webserv: [warn]: get_first_line: Request without host part" << std::endl;
		this->set_error(std::make_pair(400, "Bad request"));
	}
	else
	{
		word = recup_word(request, i);
		this->set_host(word);
	}
}
char*	Request::read_header(int client_socket)
{
	int bytes_read, msgsize;
	char *buffer;

	try
	{
		buffer = new char[MAX_LEN];
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [warn]: read_header: " << e.what() << std::endl;
		return NULL;
	}
	FIX_BROKEN_PIPE;
	memset(buffer, 0, MAX_LEN);
	msgsize = 0;
	while ((bytes_read = recv(client_socket, &buffer[msgsize], 1, 0)) > 0)
	{
		msgsize += bytes_read;
		if (msgsize > MAX_LEN - 1)
			break;
		if (msgsize > 4 && buffer[msgsize - 1] == '\n' && (strcmp(buffer + (msgsize - 1) - 1, "\n\n") == 0 || strcmp(buffer + (msgsize - 1) - 3, "\r\n\r\n") == 0))
			break;
	}
	if (msgsize <= MAX_LEN - 1)
		buffer[msgsize] = 0;
	return buffer;
}


void Request::parse_header(std::string request, Server const & server)
{
	int i = 0;
	int host = 0;
	int chunked = 0;

	lower_file(request);
	get_first_line(request, server);
	while (request[i])
	{
		if (request.compare(i, 15, "content-length:") == 0)
		{
			i += 16;
			int nb = recup_nb(request, i);
			this->set_content_length(nb);
		}
		else if (request.compare(i, 13, "content-Type:") == 0)
		{
			i += 14;
			int tmp = i;
			while (request[i] && request[i] != '\n')
				++i;
			this->set_content_type(request.substr(tmp, i - tmp));
		}
		else if (request.compare(i, 18, "transfer-encoding:") == 0)
		{
			i += 19;
			while (request[i] && request[i] != '\n')
			{
				std::string word = request.substr(i, skip_the_word(request, i) - i);
				if (word != "chunked")
				{
					chunked = 1;
					this->set_error(std::make_pair(501, "Not Implemented"));
				}
				i = skip_the_word(request, i);
				if (request[i] != '\n')
					++i;
			}
		}
		else if (request.compare(i, 5, "date:") == 0)
		{
			i += 5;
			int tmp = i;
			while (request[i] && request[i] != '\n')
				++i;
			this->set_date(request.substr(tmp, i - tmp));
		}
		else
		{
			while (request[i] && request[i] != '\n')
				++i;
			if (request[i])
				++i;
		}
	}
	if (_method == POST && _content_length == -1 && chunked == 0)
	{
		std::cerr << "webserv: [warn]: class Request: parse_header: request without content-length and chunked" << std::endl;
		this->set_error(std::make_pair(411, "Length Required"));
	}
	if (this->get_host().empty() || host > 1)
	{
		std::cerr << "webserv: [warn]: class Request: parse_header: none or too many Host field" << std::endl;
		this->set_error(std::make_pair(400, "Bad request"));
	}
	if (this->get_path().length() > 2048)
	{
		std::cerr << "webserv: [warn]: class Request: parse_header: path to long (more than 2048 characters)" << std::endl;
		this->set_error(std::make_pair(400, "Bad request"));
	}
}
