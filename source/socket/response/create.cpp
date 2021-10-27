#include "../socket.hpp"

// std::string create_header(Request & a) { std::string header; }

std::string create_error(std::string status) { return "<html>\n<head>\n<title>" + status + "</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>" + status + "</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>"; }

std::string create_response(Request & request, Server const & server, int client_socket) {
	std::string body;
	std::string response;

	if (request.get_method() == GET) {
		body = method_get(request, server);
	}
	if (request.get_method() == POST) {
		// method_post(request, server, client_socket);
		char *buffer = read_body(client_socket, 0, request.get_content_length());
		std::cout << "> body" << std::endl;
		for (int i = 0; i < request.get_content_length(); i++) {
			std::cout << buffer[i];
		}
		std::cout << std::endl;
		delete [] buffer;
	} 
	if (request.get_method() == DELETE) {
		// body = method_get(request, server);
	}
	if (request.get_error().first != 200) {
		body = create_error(to_string(request.get_error().first) + " " + request.get_error().second);
	}
	request.set_content_length(body.length());
	response += header(request);
	response += body;
	return response;
}
