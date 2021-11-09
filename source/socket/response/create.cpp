#include "../socket.hpp"
#include <pthread.h>

void send_response(Request & request, std::string body, int client_socket) {
	request.set_content_length(body.length());
	std::string response = header(request);
	response.append(body);
	write(client_socket, response.c_str(), response.length());
	close(client_socket);
}

std::string create_error(std::string status) { return "<html>\n<head>\n<title>" + status + "</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>" + status + "</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>"; }

void create_response(Request & request, Server const & server, int client_socket, int current_reading, std::vector<pthread_t> & threads) {
	int method = request.get_method();
	std::pair<int, std::string> error = request.get_error();
	pthread_t id;
	Thread *t;

	if (error.first != 200) 
		return send_response(request, create_error(to_string(error.first) + " " + error.second), client_socket);
	if (method == POST) {
		try {
			t = new Thread;
		} catch (std::bad_alloc) {
			std::cerr << "Error: bad_alloc thread in create_response";
			close(client_socket);
			return;
		}
		t->init(request, server, client_socket, current_reading);
		pthread_create(&id, NULL, method_post, t);
		threads.push_back(id);
	} else if (method == GET) {
		try {
			t = new Thread;
		} catch (std::bad_alloc) {
			std::cerr << "Error: bad_alloc thread in create_response";
			close(client_socket);
			return;
		}
		t->init(request, server, client_socket, current_reading);
		pthread_create(&id, NULL, method_get, t);
		threads.push_back(id);
	}
	else{
		std::string response, body;
		// method_delete(request, server);
		error = request.get_error();
		if (error.first != 200) // check si il n'y a pas de error_page definis
			body = create_error(to_string(error.first) + " " + error.second);
		send_response(request, body, client_socket);
	}	
}
