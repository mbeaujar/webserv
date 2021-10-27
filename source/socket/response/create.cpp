#include "../socket.hpp"
#include <pthread.h>

void send_response(std::string header, std::string body, int client_socket) {
	
}

std::string create_error(std::string status) { return "<html>\n<head>\n<title>" + status + "</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>" + status + "</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>"; }

void create_response(Request & request, Server const & server, int client_socket) {
	std::string body;
	std::string response;

	if (request.get_method() == GET) {
		body = method_get(request, server);
	}
	if (request.get_method() == POST) {
		thread a;
		pthread_t id;
		a.request = &request;
		a.server = &server;
		a.fd = client_socket;
		pthread_create(&id, NULL, method_post, &a); // crée le thread
		pthread_detach(id); // le détache pour pas avoir a le join (fix memory leaks)
		return;
	}
	if (request.get_method() == DELETE) {
		// method_delete(request, server);
	}
	if (request.get_error().first != 200) {
		body = create_error(to_string(request.get_error().first) + " " + request.get_error().second);
	}
	request.set_content_length(body.length());
	response += header(request);
	response += body;
	write(client_socket, response.c_str(), response.length());
	close(client_socket);
	std::cout << "> Response" << std::endl;
	std::cout << response << std::endl;
}
