#include "../socket.hpp"

std::string path_to_file(Request & request, Location & location, int method) {
	std::string path = request.get_path();

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
				std::cerr << "webserv: [warn]: Request path is a directory and autoindex is off in the config file" << std::endl;
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
	if (request.get_error().first >= 400) {
		std::cerr << "webserv: [warn]: parse_get: can't find location" << std::endl;
		return "";
	}
	redirect = location.get_return();
	if (redirect.first != -1) {
		request.set_return(std::make_pair(redirect.first, redirect.second));
		std::cerr << "webserv: [warn]: redirection: [" + to_string(redirect.first) + "] " + redirect.second << std::endl;
		return "";
	}
	path = path_to_file(request, location, GET);
	if (request.get_error().first >= 400) {
		std::cerr << "webserv: [warn]: parse_get: path: " << path << " does not exist" << std::endl;
		// request.set_error(std::make_pair(500, "Internal Server Error"));
		return "";
	}
	if (is_directory(path) && location.get_autoindex() == true) {
		response = autoindex_on(path, location.get_root(), request.get_host(), server.get_current_port());
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
	std::string body;

	try {
		body = parse_get(a->request, a->server, a->client_socket);
	} catch (std::exception &e) {
		std::cerr << "webserv: [warn]: method_get: " << e.what() << std::endl;
		a->request.set_error(std::make_pair(500, "Internal Server Error"));
	}
	send_response(a->request, body, a->client_socket, a->server);
	delete a;
	return NULL;
}
