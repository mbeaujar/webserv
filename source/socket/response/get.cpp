#include "../socket.hpp"

std::string path_to_file(Request & request, Location location) {
	std::string r_root = location.get_root();
	std::string path = r_root + request.get_path();

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
			if (location.get_autoindex() == false) {
				std::cerr << "Error: is a directory + autoindex is off" << std::endl;
				request.set_error(std::make_pair(404, "Not Found"));
			}
			std::cerr << "DEBUG: path autoindex " << path << std::endl;
		}
	}
	else
		request.set_error(std::make_pair(404, "Not Found"));
	return path;
}

std::string method_get(Request & request, Server const & server, int client_socket) {
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
	path = path_to_file(request, location);
	if (request.get_error().first != 200)
		return "";
	if (is_directory(path) && location.get_autoindex() == true) {
		response = autoindex_on(path);
	} else if (is_cgi(path, location.get_fastcgi_ext()) == true) {
		response = call_cgi(request, client_socket, path, "GET", location.get_fastcgi());
		response = parse_cgi(request, response);
	} else
		response = get_file_content(path);
	return response;
}
