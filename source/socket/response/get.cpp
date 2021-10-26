#include "../socket.hpp"

std::string path_to_file(Request & request, Location location) {
	std::string r_root = location.get_root();
	std::string path;

	if (r_root.empty()) {
		r_root = "/var/www/html";
	}
	if (r_root[r_root.length()-1] == '/') {
		r_root.erase(r_root.end());
	}
	if (request.get_path() == "/")
		path = r_root;
	else
		path = r_root + request.get_path();
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

std::string method_get(Request & request, Server const & server) {
	std::string	html;
	std::string	response;
	Location	location;

	if ((location = search_location(request.get_path(), server)).get_return().first == 1) {
		std::cerr << "Error: Can't find a location for the path" << std::endl;
		request.set_error(std::make_pair(404, "Not Found"));
		return "";
	}
	if (location.get_method(GET) == false) {
		struct s_method m = location.get_methods();
		request.set_methods(m);
		request.set_error(std::make_pair(405, "Method Not Allowed"));
		std::cerr << "Error: Method not allowed" << std::endl;
		return "";
	}
	std::pair<int, std::string> redirect = location.get_return();
	if (redirect.first != -1) {
		request.set_return(std::make_pair(redirect.first, redirect.second));
		std::cerr << "Info: redirection" << std::endl;
		return "";
	}
	std::string path = path_to_file(request, location);
	if (request.get_error().first == 200)
		response = get_file_content(path);
	return response;
}


