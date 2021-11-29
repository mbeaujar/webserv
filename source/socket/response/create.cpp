#include "../socket.hpp"
#include <pthread.h>

void send_response(Request &request, std::string body, int client_socket, Server const &server)
{
	std::pair<int, std::string> error;
	std::string response;

	error = request.get_error();
	if (ISERROR(error.first))
		body = create_error(to_string(error.first) + " " + error.second, server, error.first);
	request.set_content_length(body.length());

	response = header(request);

	response.append(body);
	std::cerr << std::endl;
	std::cerr << "RESPONSE: " << std::endl
			  << response << std::endl;
	std::cerr << "------------------------------------------------------" << std::endl;
	write(client_socket, response.c_str(), response.length());
	close(client_socket);
}

std::string create_error(std::string status, Server const &server, int &error)
{
	std::string path = server.find_error(error);
	if (path != "")
		if (file_exist(path))
			return get_file_content(path);
	return "<html>\n<head>\n<title>" + status + "</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>" + status + "</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>";
}

void create_response(Request &request, Server const &server, int client_socket)
{
	int method = request.get_method();
	std::pair<int, std::string> error;

	error = request.get_error();
	if (ISERROR(error.first))
		return send_response(request, create_error(to_string(error.first) + " " + error.second, server, error.first), client_socket, server);
	if (method == POST)
		method_post(server, request, client_socket);
	else if (method == GET)
		method_get(server, request, client_socket);
	else if (method == DELETE)
		method_delete(request, server, client_socket);
	else
	{
		//ERREUR : methode inconnu le programme doit exit (return un code d'erreur logique)
	}
}
