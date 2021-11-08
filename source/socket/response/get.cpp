#include "../socket.hpp"

std::string path_to_file(Request & request, Location & location, int method) {
	std::string path = location.get_root() + request.get_path();

	if (file_exist(path)) {
		if (is_directory(path)) {
			if (path[path.length() - 1] != '/')
				path.insert(path.end(), '/');
			std::vector<std::string> index = location.get_index();
			std::vector<std::string>::iterator it = index.begin(), ite = index.end();
			while (it != ite) {
				std::string path_index = path + *it;
				if (file_exist(path_index) && !is_directory(path_index))
					return path_index;
				++it;
			}
			if (location.get_autoindex() == false && method == GET) {
				std::cerr << "Error: Request path is a directory + autoindex off in config file" << std::endl;
				request.set_error(std::make_pair(404, "Not Found"));
			}
		}
	}
	else if (method == GET)
		request.set_error(std::make_pair(404, "Not Found"));
	return path;
}

std::string parse_get(Request & request, Server const & server, int client_socket) {
	std::pair<int, std::string> redirect;
	std::string	html, response, path;
	Location	location;

	location = find_location(request, server, GET);
	if (request.get_error().first != 200)
		return "";
	redirect = location.get_return();
	if (redirect.first != -1) {
		request.set_return(std::make_pair(redirect.first, redirect.second));
		std::cerr << "Warning: redirection: [" + to_string(redirect.first) + "] " + redirect.second << std::endl;
		return "";
	}
	path = path_to_file(request, location, GET);
	if (request.get_error().first != 200)
		return "";
	if (is_directory(path) && location.get_autoindex() == true) {
		response = autoindex_on(path);
	} else if (is_cgi(path, location.get_fastcgi_ext()) == true) {
		response = call_cgi(request, client_socket, path, "GET", location.get_fastcgi());
		if (response.empty() == true) 
			request.set_error(std::make_pair(502, "Bad Gateway"));
		else
			response = parse_cgi(request, response);
	} else
		response = get_file_content(path);
	return response;
}

void *method_get(void *arg) {
	Thread *a = reinterpret_cast<Thread*>(arg);
	std::pair<int, std::string> error;

	std::string body = parse_get(a->request, a->server, a->client_socket);
	error = a->request.get_error();
	if (error.first != 200)
		body = create_error(to_string(error.first) + " " + error.second);
	send_response(a->request, body, a->client_socket);
	delete a;
	return NULL;
}
