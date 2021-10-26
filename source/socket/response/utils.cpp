#include "../socket.hpp"


bool file_exist(std::string filename) {
	std::ifstream file;
	file.open(filename.c_str());
	if (file) {
		file.close();
		return true;
	}
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
	tmp.set_return(1, "f uck le a-word");

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

