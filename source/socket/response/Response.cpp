#include "Response.hpp"

Response::Response(int & client_socket, Request & request, Server & server) :
	_client_socket(client_socket),
	_request(request),
	_server(server),
	_response(),
	_method(NULL),
	_content_length() {}


Response::Response(Response const & copy) :
	_client_socket(copy._client_socket),
	_request(copy._request),
	_server(copy._server),
	_response(copy._response),
	_method(copy._method),
	_content_length(copy._content_length) {}

Response::~Response()
{
	if (_method != NULL)
		delete _method;
}

Response & Response::operator=(Response const & copy)
{
    if (this != &copy)
    {
		_client_socket = copy._client_socket;
		_request = copy._request;
		_server = copy._server;
		_response = copy._response;
		_method = copy._method;
		_content_length = copy._content_length;
    }
    return *this;
}

void Response::execute(void)
{
	try
	{
		std::string body;

		if (ISERROR(_request.get_error().first) == false)
		{
			this->create_method(_request.get_method());
			if (_method != NULL)
			{
				_method->execute();
				if (_request.get_method() == GET)
					body = _method->get_body();
			}
		}
		if (ISERROR(_request.get_error().first) == true)
			body = this->error_html();
		// std::cerr << body << std::endl;
		this->create_header(); 
		_response.append(body);
		_content_length = body.length();
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: [warn]: class Response: execute: " << e.what() << '\n';
		_request.set_error(std::make_pair(500, "Internal Server Error"));
	}
	this->send_response();
}

// --------------------- PRIVATE ----------------- //

void Response::send_response(void)
{
	if (send(_client_socket, _response.c_str(), _response.length(), MSG_CONFIRM) == -1)
		std::cerr << "webserv: [warn]: class Response: send_response: can't write to client" << "\n";
	if (shutdown(_client_socket, SHUT_RDWR) == -1)
		std::cerr << "webserv: [warn]: class Response: send_response: shutdown can't shut client and socket" << "\n";
	close(_client_socket);
}

std::string Response::error_html(void)
{
	std::pair<int, std::string> error = _request.get_error();
	std::string status = to_string(error.first) + error.second;
	std::string path = _server.find_error(error.first);
	if (path.empty() == false)
	{
		if (file_exist(path))
			return get_file_content(path);
	}
	path = "<html>\n";
	path += "	<head>\n";
	path += "		<title>" + status + "</title>\n";
	path += "	</head>\n";
	path += "	<body bgcolor=\"white\">\n";
	path += "		<center>\n";
	path += "			<h1>" + status + "</h1>\n";
	path += "		</center>\n";
	path += "		<hr>\n";
	path += "		<center>webserv/1.0.0 (Ubuntu)</center>\n";
	path += "	</body>\n";
	path += "</html>\n";
	return path;
}

void Response::create_method(int & method)
{
	try
	{
		if (method == POST)
			;// _method = new Post();
		else if (method == GET)
			_method = new Get(_server, _request, _client_socket);
		else if (method == DELETE)
			_method = new Delete(_server, _request, _client_socket);
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: [warn]: class Response: create_method: " << e.what() << '\n';
		_method = NULL;
	}
}


std::string Response::allow_method(void)
{
	std::string method;
	s_method m = _request.get_methods();

	if (m.m_get == true)
		method += " GET";
	if (m.m_delete == true)
	{
		if (method != "")
			method += ",";
		method += " DELETE";
	}
	if (m.m_post == true)
	{
		if (method != "")
			method += ",";
		method += " POST";
	}
	return method;
}

std::string Response::get_hour_date(void)
{
	char buf[1000];
	time_t now = time(0);
	tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S GMT", &tm);
	std::string date(buf);
	return date;
}

std::string get_last_modified(std::string & path)
{
	struct stat stats;
	tm *tm;
	char buf[1000];

	memset(&stats, 0, sizeof(stats));
	stat(path.c_str(), &stats);
	tm = gmtime(&stats.st_mtime);
	strftime(buf, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	std::string date(buf);
	return date;
}

void Response::create_header(void)
{
	std::pair<int, std::string> & redirect  = _request.get_return();
	std::pair<int, std::string> & error     = _request.get_error();

	if (ISREDIRECT(redirect.first))
	{
		error.first = redirect.first;
		error.second = "Moved Permanently";
	}
	_response = "HTTP/1.1 " + to_string(error.first) + " " + error.second + "\r\n";
	_response += "Server: " + _request.get_host() + "\r\n";
	_response += "Date: " + this->get_hour_date() + "\r\n";
	_response += "Connection: close\r\n";
	if (_request.get_content_type().empty() == false)
		_response += "Content-Type: " + _request.get_content_type() + "\r\n";
	if (_request.get_content_length() == GET)
		_response += "Content-length: " + to_string(_content_length) + "\r\n";
	if (file_exist(_request.get_file()) == true)
		_response += "Last-Modified: " + get_last_modified(_request.get_file()) + "\r\n";
	if (error.first == 405)
		_response += "Allow: " + this->allow_method() + "\r\n";
	if (ISREDIRECT(redirect.first) || error.first == 201)
	{
		if (ISREDIRECT(redirect.first))
			_response += "Location: " + redirect.second + "\r\n";
		else
			_response += "Location: " + _request.get_path() + "\r\n";
	}
	_response += "\r\n"; // blank line
}
