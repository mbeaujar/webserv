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

void *method_post(void *arg) {
	thread *a = static_cast<thread*>(arg);
	Server const & server = *(a->server);
	Request & request = *(a->request);
	int client_socket = a->fd;
	char *buffer;
	Location location;
	
	if (request.get_error().first != 200)
		return NULL;
	if ((location = search_location(request.get_path(), server)).get_return().first == 1) {
		std::cerr << "Error: Can't find a location for the path" << std::endl;
		request.set_error(std::make_pair(404, "Not Found"));
		return NULL;
	}
	if (location.get_method(POST) == false) {
		struct s_method m = location.get_methods();
		request.set_methods(m);
		request.set_error(std::make_pair(405, "Method Not Allowed"));
		std::cerr << "Error: Method not allowed" << std::endl;
		return NULL;
	}
	buffer = read_body(client_socket, 0, request.get_content_length());
	delete [] buffer;
	return NULL;
}

// In any case, if a POST request is made with a Content-Type which 
// cannot be handled by the application, it should return a 415 status-code.