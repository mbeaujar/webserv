#include "../socket.hpp"

bool file_exist(std::string filename) {
	std::ifstream file;
	file.open(filename.c_str());
	if (file) {
		file.close();
		return true;
	}
	file.close();
	return false;
}

bool is_directory(std::string path) {
	struct stat s;
	if (stat(path.c_str(), &s) == 0) {
		if (s.st_mode & S_IFDIR)
			return true;
	}
	return false;
}

std::string cut_filename(std::string path) {
	int i = path.length();
	while (i > 0 && path[i] != '/')
		i--;
	std::string filename = path.substr(i + 1, path.length() - (i + 1));
	if (filename.empty() == true)
		return "/";
	return filename;
}

std::string cut_path(std::string path) {
	int i = path.length();
	while (i > 0 && path[i] != '/')
		i--;
	return path.substr(0, i);
}

int remove_file(char const *path) {
	int ret = remove(path);
	if (ret == -1)
		std::cerr << "Error: remove fail for path: " << path << std::endl;
	return ret;
}

std::string path_in_common(std::string location, std::string & path) {
	int len = 0;
	
	while (path[len] && location[len] && path[len] == location[len])
		len++;
	if (path[len] != '/' && path.length() != location.length()) {
		while (len > 0 && path[len] != '/')
			len--;
	}
	return path.substr(0, len);
}

Location search_location(std::string path, Server const & server) {
	Location tmp;
	std::string longuest_path = "/";
	std::map<std::string, Location> all = server.get_all_location();
	std::map<std::string, Location>::iterator it = all.begin(), ite;
	tmp.set_return(1, "fuck le a-word");

	if ((ite = all.find("/")) != all.end()) {
		tmp = ite->second;
		ite = all.end();
	}
	while (it != ite) {
		std::string common = path_in_common(it->first, path);
		if (common.length() > longuest_path.length()) {
			tmp = it->second;
			longuest_path = common;
		}
		++it;
	}
	return tmp;
}

Location find_location(Request & request, Server const & server, int method) {
	Location location;

	if ((location = search_location(request.get_path(), server)).get_return().first == 1) {
		std::cerr << "Error: Can't find a location for the path" << std::endl;
		request.set_error(std::make_pair(404, "Not Found"));
		return location;
	}
	// check si la method delete est autorisé
	if (location.get_method(method) == false) {
		struct s_method m = location.get_methods();
		request.set_methods(m);
		request.set_error(std::make_pair(405, "Method Not Allowed"));
		std::cerr << "Error: Method not allowed" << std::endl;
		return location;
	}
	request.set_path(location.get_root() + request.get_path());
	return location;
}

bool is_cgi(std::string & path, std::string extension_cgi) {
	size_t path_len = path.length();
	size_t ext_len = extension_cgi.length();

	if (extension_cgi == "" || path_len <= ext_len)
		return false;
	std::string extension_path = path.substr(path_len - ext_len, ext_len);
	if (extension_path == extension_cgi)
		return true;
	return false;
}