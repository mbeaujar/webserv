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
		}
	}
	else
		request.set_error(std::make_pair(404, "Not Found"));
	return path;
}

std::string method_get(Request & request, Server const & server, int client_socket) {
	std::string	html;
	std::string	response;
	Location	location;

	location = find_location(request, server, GET);
	if (request.get_error().first != 200)
		return "";
	std::pair<int, std::string> redirect = location.get_return();
	if (redirect.first != -1) {
		request.set_return(std::make_pair(redirect.first, redirect.second));
		std::cerr << "Warning: redirection" << std::endl;
		return "";
	}
	std::string path = path_to_file(request, location);
	if (request.get_error().first == 200)
		response = get_file_content(path);
	std::string cgi_extension = location.get_fastcgi_ext();
	if (cgi_extension.empty() == false) {
		if (path.length() > cgi_extension.length()) {
			size_t len = path.length() - cgi_extension.length();
			if (len >= 0 && path.compare(len, cgi_extension.length(), cgi_extension) == 0)
				response = call_cgi(request, location.get_fastcgi(), client_socket, response, "GET", path);
		}
	}
	return response;
}

