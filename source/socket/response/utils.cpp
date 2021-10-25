#include "../socket.hpp"
#include <sys/stat.h>

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

std::string path_in_common(std::string location, std::string path) {
	int len = 0;
	for (int i = 0; path[i] && location[i] && path[i] == location[i]; i++)
		len++;
	if (path[len] != '/' && path.length() != location.length()) {
		while (len > 0 && path[len] != '/')
			len--;
	}
	return path.substr(0, len);
}

Location search_location(std::string path, Server const & server) {
	std::map<std::string, Location> all = server.get_all_location();
	std::map<std::string, Location>::iterator it = all.begin(), ite;
	Location tmp;
	std::string longuest_path = "/";
	tmp.set_return(1, "fuckali");

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

std::string hour_date() {

	std::string	date;
	char 		buf[1000];
	time_t		now = time(0);
	struct tm tm = *gmtime(&now);

	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	date = buf;
	return date;
}

std::string get_file_content(std::string filename) 
{
    std::string	    content, line;
    std::ifstream	file;

    file.open(filename.c_str());
    while (std::getline(file, line)) {
        content.append(line);
		if (!file.eof())
			content.append("\n");
	}
    return content;
}

