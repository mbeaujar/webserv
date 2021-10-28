#include "../socket.hpp"
#include <pthread.h>

void send_response(Request & request, std::string body, int client_socket) {
	request.set_content_length(body.length());
	std::string response = header(request);
	response += body;
	write(client_socket, response.c_str(), response.length());
	close(client_socket);
}

std::string create_error(std::string status) { return "<html>\n<head>\n<title>" + status + "</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>" + status + "</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>"; }

void create_response(Request & request, Server const & server, int client_socket) {
	int method = request.get_method();
	std::pair<int, std::string> error = request.get_error();

	if (error.first != 200) 
		return send_response(request, create_error(to_string(error.first) + " " + error.second), client_socket);
	if (method == POST) {
		pthread_t id;
		Thread *t = new Thread;
		t->init(request, server, client_socket);
		pthread_create(&id, NULL, method_post, t);
		pthread_detach(id);
	} else {
		std::string response, body;

		if (method == GET) 
			body = method_get(request, server);
		else
			;// method_delete(request, server);
		error = request.get_error();
		if (error.first != 200)
			body = create_error(to_string(error.first) + " " + error.second);
		send_response(request, body, client_socket);
	}	
}

