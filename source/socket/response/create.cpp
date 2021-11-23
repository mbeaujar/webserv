#include "../socket.hpp"
#include <pthread.h>

void send_response(Request & request, std::string body, int client_socket, Server const & server) {
	std::pair<int, std::string> error;
	std::string response;

	request.set_content_length(body.length());
	response = header(request);
	error = request.get_error();
	if (error.first != 200)
		response += create_error(to_string(error.first) + " " + error.second, server, error.first);
	else
		response.append(body);
	// std::cout << "RESPONSE: " << std::endl << response << std::endl;
	write(client_socket, response.c_str(), response.length());
	close(client_socket);
}

std::string create_error(std::string status, Server const & server, int & error) {
	std::string path = server.find_error(error);
	if (path != "")
		if (file_exist(path))
			return get_file_content(path);
	return "<html>\n<head>\n<title>" + status + "</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>" + status + "</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>";
}

void create_response(Request & request, Server const & server, int client_socket, int current_reading, std::vector<pthread_t> & threads) {
	int method = request.get_method();
	std::pair<int, std::string> error = request.get_error();
	pthread_t id;
	Thread *t;

	if (error.first != 200) 
		return send_response(request, create_error(to_string(error.first) + " " + error.second, server, error.first), client_socket, server);
	if (method == POST) {
		try {
			t = new Thread;
		} catch (std::exception &e) {
			std::cerr << "webserv: [warn]: create_response POST: " << e.what() << std::endl;
			close(client_socket);
			return;
		}
		t->init(request, server, client_socket, current_reading);
		pthread_create(&id, NULL, method_post, t);
		threads.push_back(id);
	} else if (method == GET) {
		try {
			t = new Thread;
		} catch (std::exception &e) {
			std::cerr << "webserv: [warn]: create_response GET: " << e.what() << std::endl;
			close(client_socket);
			return;
		}
		t->init(request, server, client_socket, current_reading);
		pthread_create(&id, NULL, method_get, t);
		threads.push_back(id);
	}
	else if (method == DELETE) {
		method_delete(request, server, client_socket);
	}	
	else {
		//ERREUR : methode inconnu le programme doit exit (return un code d'erreur logique)
	}
}
