#include "../socket.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>

// opendir readir

// echange si a < b
bool dirent_comp(struct dirent* a, struct dirent *b) 
{
     if (strcmp(a->d_name, b->d_name) <= 0)
        return true;
    return false;
}

std::string current_host(std::string const & host, int const & port) {
	int i = 0;
	while (host[i] && host[i] != ':')
		i++;
	if (host[i] == ':')
		return host;
	return host + ":" + to_string(port);
}

std::string clear_dirname(std::string const & dirname, std::string const & root) {
	int i = 0;
	while (dirname[i] && dirname[i] == root[i])
		i++;
	return dirname.substr(i, dirname.length() - i);
}
 
std::string get_data(std::string const & path_to_file, std::string const & filename) {
    std::vector<std::string> words;
	int x = 0, pos = 0;

	if (filename == "..")
		return "";
	std::string path = get_last_modified(path_to_file + filename);
	while (path[pos] && isdigit(path[pos]) == false)
		pos++;
    for (x = pos; path[pos]; pos++) {
        if (path[pos] == ' ') {
            words.push_back(path.substr(x, pos - x));
			while (path[pos] && path[pos] == ' ')
				++pos;
			x = pos;
		}
    }
	words[3].erase((words[3].length() - 1) - 2, 3);
    return words[0] + "-" + words[1] + "-" + words[2] + " " + words[3];
}

std::string put_file_name(std::string file) {
    for (int i = 0; file[i]; i++)
    {
        if (i == 47 || i == 48)
            file[i] = '.';
        if (i == 49)
            file[i] = '>';
        if (i > 49)
            file[i] = 0;
    }
    int len = 50 - file.length();
    while (len--)
        file.push_back(' ');
    return file;
}

std::string put_space_data(std::string const & path_to_file, std::string const & filename) {
    return get_data(path_to_file, filename) + "\n";
}

std::string create_element(struct dirent *file, std::string const & dir_name, std::string const & root, bool const & is_dir, std::string const & host, int const & port)
{
	return "<a href= \"http://" + current_host(host, port) + clear_dirname(dir_name, root) + file->d_name + "\">" + put_file_name(file->d_name) + (is_dir == true ? "/" : "") + "</a>" + put_space_data(dir_name, file->d_name);
}

std::string create_list_element(std::vector<struct dirent*> & list, std::string const & dir_name, std::string const & root, bool const & is_dir, std::string const & host, int const & port) {
	std::string response;

    std::sort(list.begin(), list.end(), dirent_comp);
	std::vector<struct dirent*>::iterator it = list.begin(), ite = list.end();
    while (it != ite) {
		response += create_element(*it, dir_name, root, is_dir, host, port);
        ++it;
    }
	return response;
}

std::string listing(std::string const & path, std::string const & root, std::string const & host, int const & port)
{
    std::string response;
    std::string dir_name = path;
    std::vector<struct dirent*> directory;
    std::vector<struct dirent*> file;

	if (dir_name[dir_name.length() - 1] != '/')
		dir_name.insert(dir_name.end(), '/');

    DIR *dir = opendir(path.c_str());
    if (dir == NULL) {
        std::cerr << "Error: can't opendir: " << strerror(errno) << std::endl;
        return "";
    }
    for (struct dirent *for_dir = readdir(dir); for_dir != NULL; for_dir = readdir(dir)) {
		if (strcmp(for_dir->d_name, "..") == 0)
			response += create_element(for_dir, dir_name, root, true, host, port);
        else if (strcmp(for_dir->d_name, ".") != 0) {
			if (is_directory(dir_name + for_dir->d_name))
				directory.push_back(for_dir);
			else
				file.push_back(for_dir);
		}
    }
	response += create_list_element(directory, dir_name, root, true, host, port);
	response += create_list_element(file, dir_name, root, false, host, port);
	closedir(dir);
    return response;
}

std::string autoindex_on(std::string const & path, std::string const & root, std::string const & host, int const & port)
{
    std::string response;
	std::string path_to_dir = path;
	path_to_dir.erase(--path_to_dir.end());

    response = "<html>\n";
    response += "<head>\n";
    response += "<title>" + path + "</title>\n";
    response += "</head>\n";
    response += "<body bgcolor=\"white\">\n";
    response += "<h1>Index of " + clear_dirname(path_to_dir, root) + "/</h1>\n";
    response += "<hr>\n";
    response += "<pre>";
    response += listing(path, root, host, port);
    response += "</pre>";
    response += "<hr>\n";
    response += "</body>\n";
    response += "</html>";
    return response;
}
