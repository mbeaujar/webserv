#include "../socket.hpp"

char    *read_body(int client_socket, int limit, int buffersize) {
	int     bytes_read;
	int     msgsize = 0;
	char    *buffer = new char[buffersize];

	memset(buffer, 0, buffersize);
	if (limit == 0)
		limit = buffersize;
	while ((bytes_read = read(client_socket, buffer + msgsize, buffersize - msgsize - 1)) > 0) {
		msgsize += bytes_read;
        if (msgsize > buffersize - 1)
	 		break;
	}
	return buffer;
}

void method_post(Request & request, Server const & server, int client_socket) { // + struct
	std::string	html;
	Location	location;
	char *buffer = read_body(client_socket, 0, request.get_content_length());

	if ((location = search_location(request.get_path(), server)).get_return().first == 1) {
		std::cerr << "Error: Can't find a location for the path" << std::endl;
		request.set_error(std::make_pair(404, "Not Found"));
		return;
	}
	if (location.get_method(POST) == false) {
		struct s_method m = location.get_methods();
		request.set_methods(m);
		request.set_error(std::make_pair(405, "Method Not Allowed"));
		std::cerr << "Error: Method not allowed" << std::endl;
		return;
	}
	std::pair<int, std::string> redirect = location.get_return();
	if (redirect.first != -1) {
		request.set_return(std::make_pair(redirect.first, redirect.second));
		std::cerr << "Info: redirection" << std::endl;
		return;
	}
	// std::string path = path_to_file(request, location);
	// if (request.get_error().first == 200)
	// 	 response = get_file_content(path);
	delete [] buffer;
}
