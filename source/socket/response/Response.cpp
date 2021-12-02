#include "Response.hpp"

Response::Response(int client_socket, Request & request, Server & server) :
	_client_socket(client_socket),
	_request(request),
	_server(server) {}


Response::Response(Response const & copy) :
	_client_socket(copy._client_socket),
	_request(copy._request),
	_server(copy._server) {}

Response::~Response() {}

Response & Response::operator=(Response const & copy) {
    if (this != &copy)
    {
		_client_socket = copy._client_socket;
		_request = copy._request;
		_server = copy._server;
    }
    return *this;
}

std::string Response::allow_method(Request &request)
{
	std::string method;
	struct s_method m = request.get_methods();

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

// std::string Respose::header(Request & request)
// {
// 	std::string header;
// 	std::pair<int, std::string> redirect = request.get_return();

// 	if (ISREDIRECT(redirect.first))
// 		request.set_error(std::make_pair(redirect.first, "Moved Permanently"));
// 	header += "HTTP/1.1 " + to_string(request.get_error().first) + " " + request.get_error().second + "\r\n";
// 	header += "Date: " + hour_date() + "\r\n";
// 	header += "Server: " + request.get_host() + "\r\n";
// 	header += "Connection: close\r\n";
// 	if (request.get_content_type().empty() == false)
// 		header += "Content-Type: " + request.get_content_type() + "\r\n";
// 	if (request.get_method() == GET)
// 		header += "Content-length: " + to_string(request.get_content_length()) + "\r\n";
// 	if (file_exist(request.get_file()))
// 		header += "Last-Modified: " + get_last_modified(request.get_file()) + "\r\n";
// 	if (request.get_error().first == 405)
// 		header += "Allow:" + allow_method(request) + "\r\n";
// 	if (redirect.first != -1 || request.get_error().first == 201)
// 	{
// 		if (redirect.first != -1)
// 			header += "Location: " + redirect.second + "\r\n";
// 		else
// 			header += "Location: " + request.get_path() + "\r\n";
// 	}
// 	header += "\r\n"; // blank line
// 	return header;
// }
