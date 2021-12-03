#include "Autoindex.hpp"

Autoindex::Autoindex(std::string const & path, std::string const & root, std::string const & host, int const & port)
{
    this->autoindex_on(path, root, host, port);
}

Autoindex::Autoindex(Autoindex const & src) { *this = src; }

Autoindex::~Autoindex() {}

Autoindex & Autoindex::operator=(Autoindex const & rhs)
{
    if (this != &rhs)
    {

    }
    return *this;
}

// echange si a < b
bool Autoindex::dirent_comp(struct dirent *a, struct dirent *b)
{
	if (strcmp(a->d_name, b->d_name) <= 0)
		return true;
	return false;
}

std::string Autoindex::current_host(std::string const & host, int const & port)
{
	int i = 0;
	while (host[i] && host[i] != ':')
		i++;
	if (host[i] == ':')
		return host;
	return host + ":" + to_string(port);
}

std::string Autoindex::clear_dirname(std::string const & dirname, std::string const & root)
{
	int i = 0;
	while (dirname[i] && dirname[i] == root[i])
		i++;
	return dirname.substr(i, dirname.length() - i);
}

std::string Autoindex::get_data(std::string const & path_to_file, std::string const & filename, std::string const & file)
{
	std::vector<std::string> words;
	int x = 0, pos = 0;

	if (filename == "../")
		return "";
	std::string path = get_last_modified(path_to_file + file);
	while (path[pos] && isdigit(path[pos]) == false)
		pos++;
	for (x = pos; path[pos]; pos++)
	{
		if (path[pos] == ' ')
		{
			words.push_back(path.substr(x, pos - x));
			while (path[pos] && path[pos] == ' ')
				++pos;
			x = pos;
		}
	}
	words[3].erase((words[3].length() - 1) - 2, 3);
	return words[0] + "-" + words[1] + "-" + words[2] + " " + words[3];
}

std::string Autoindex::put_file_name(std::string file, bool const & is_dir)
{
	file += (is_dir == true ? "/" : "");
	if (file.length() > 49)
	{
		file[47] = '.';
		file[48] = '.';
		file[49] = '>';
		return (file.substr(0, 50));
	}
	return file;
}

off_t Autoindex::file_size(const char *filename)
{
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;
	std::cerr << "webserv: [warn]: file_size: Cannot determine size of " << filename << " : " << strerror(errno) << std::endl;
	return 0;
}

std::string Autoindex::get_file_size(std::string const & path_to_file, std::string const & file, bool const & is_dir)
{
	if (is_dir == true)
		return "-";
	std::string path = path_to_file + file;
	return to_string(file_size(path.c_str()));
}

std::string Autoindex::put_space(std::string space) { return std::string(20 - space.length() < 0 ? 0 : 20 - space.length(), ' ') + space; }

std::string Autoindex::put_space_data(std::string const & path_to_file, std::string const & filename, std::string const & file, bool const & is_dir)
{
	std::string space, size;
	int len = 51 - filename.length();
	while (len-- > 0 && filename != "../")
		space.push_back(' ');
	if (filename == "../")
		return "\n";
	size = get_file_size(path_to_file, file, is_dir);
	return space + get_data(path_to_file, filename, file) + put_space(size) + "\n";
}

std::string Autoindex::create_element(struct dirent *file, std::string const & dir_name, std::string const & root, bool const & is_dir, std::string const & host, int const & port)
{
	return "<a href= \"http://" + current_host(host, port) + clear_dirname(dir_name, root) + file->d_name + "\">" + put_file_name(file->d_name, is_dir) + "</a>" + put_space_data(dir_name, put_file_name(file->d_name, is_dir), file->d_name, is_dir);
}

std::string Autoindex::create_list_element(std::vector<struct dirent *> & list, std::string const & dir_name, std::string const & root, bool const & is_dir, std::string const & host, int const & port)
{
	std::string response;

	std::sort(list.begin(), list.end(), dirent_comp);
	std::vector<struct dirent *>::iterator it = list.begin(), ite = list.end();
	while (it != ite)
	{
		response += create_element(*it, dir_name, root, is_dir, host, port);
		++it;
	}
	return response;
}

std::string Autoindex::listing(std::string const & path, std::string const & root, std::string const & host, int const & port)
{
	std::string response;
	std::string dir_name = path;
	std::vector<struct dirent *> directory;
	std::vector<struct dirent *> file;

	if (dir_name[dir_name.length() - 1] != '/')
		dir_name.insert(dir_name.end(), '/');

	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
	{
		std::cerr << "Error: can't opendir: " << strerror(errno) << std::endl;
		return "";
	}
	for (struct dirent *for_dir = readdir(dir); for_dir != NULL; for_dir = readdir(dir))
	{
		if (strcmp(for_dir->d_name, "..") == 0)
			response += create_element(for_dir, dir_name, root, true, host, port);
		else if (strcmp(for_dir->d_name, ".") != 0)
		{
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

void Autoindex::autoindex_on(std::string const & path, std::string const & root, std::string const & host, int const & port)
{
	std::string path_to_dir = path;
	path_to_dir.erase(--path_to_dir.end());

	_response = "<html>\n";
	_response += "<head>\n";
	_response += "<title>" + path + "</title>\n";
	_response += "</head>\n";
	_response += "<body bgcolor=\"white\">\n";
	_response += "<h1>Index of " + clear_dirname(path_to_dir, root) + "/</h1>\n";
	_response += "<hr>\n";
	_response += "<pre>";
	_response += listing(path, root, host, port);
	_response += "</pre>";
	_response += "<hr>\n";
	_response += "</body>\n";
	_response += "</html>";
}
