#include "Autoindex.hpp"
#include <algorithm>

Autoindex::Autoindex(std::string & path_file, std::string & request_path, std::string const & host, int const port) :
	_path_file(path_file),
	_request_path(request_path),
	_host(host),
	_port(port),
	_body(),
	_dirname(),
	_directory(),
	_file()
{
	if (_request_path.length() > 1 && _request_path[_request_path.length() - 1] != '/')
		_request_path.push_back('/');
	if (_path_file.length() > 1 && _path_file[_path_file.length() - 1] != '/')
		_path_file.push_back('/');
}

Autoindex::Autoindex(Autoindex const & copy) :
	_path_file(copy._path_file),
	_request_path(copy._request_path),
	_host(copy._host),
	_port(copy._port),
	_body(copy._body),
	_dirname(copy._dirname),
	_directory(copy._directory),
	_file(copy._file)
{
	if (_request_path.length() > 1 && _request_path[_request_path.length() - 1] != '/')
		_request_path.push_back('/');
	if (_path_file.length() > 1 && _path_file[_path_file.length() - 1] != '/')
		_path_file.push_back('/');
}

Autoindex::~Autoindex() {}

Autoindex & Autoindex::operator=(Autoindex const & copy)
{
    if (this != &copy)
    {
		_path_file = copy._path_file;
		_request_path = copy._request_path;
		// _host = copy._host; <- const can't init
		// _port = copy._port; <- same
		_body = copy._body;
		_dirname = copy._dirname;
		_directory = copy._directory;
		_file = copy._file;
    }
    return *this;
}

std::string Autoindex::get_content(void)
{
	this->fill_lists();
	this->create_list_element(_directory, true);
	this->create_list_element(_file, false);
	std::string response;

	response += "<html>\n";
	response += "<head>\n";
	response += "<title>" + _request_path + "</title>\n";
	response += "</head>\n";
	response += "<body bgcolor=\"white\">\n";
	response += "<h1>Index of " + _request_path + "</h1>\n";
	response += "<hr>\n";
	response += "<pre>" + _body + "</pre>";
	response += "<hr>\n";
	response += "</body>\n";
	response += "</html>";
	return response;
}


// ------------------------ PRIVATE --------------------- //

bool dirent_comp(struct dirent *a, struct dirent *b)
{
	if (strcmp(a->d_name, b->d_name) <= 0)
		return true;
	return false;
}

void Autoindex::fill_lists(void)
{
	DIR *dir = opendir(_path_file.c_str());
	if (dir == NULL)
		std::cerr << "webserv: [warn]: class Autoindex: fill_lists: can't open dir" << "\n";
	else
	{
		for (dirent *for_dir = readdir(dir); for_dir != NULL; for_dir = readdir(dir))
		{
			if (strcmp(for_dir->d_name, "..") == 0)
				_body += this->create_element(for_dir->d_name, true);
			else if (strcmp(for_dir->d_name, ".") != 0)
			{
				std::string path_file = _path_file + for_dir->d_name;
				if (is_directory(path_file))
					_directory.push_back(for_dir);
				else
					_file.push_back(for_dir);
			}
		}
		closedir(dir);
	}
}

std::string Autoindex::get_date(std::string & filename)
{
	int x = 0, pos = 0;
	std::vector<std::string> words;

	std::string path = _path_file + filename;
	path = get_last_modified(path);
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

std::string Autoindex::information(int len_name, std::string & filename, bool const & is_dir)
{
	std::string space_before(51 - len_name > 0 ? 51 - len_name : 0, ' ');
	struct stat st;

	if (stat(std::string(_path_file + filename).c_str(), &st) == 0)
	{
		std::string size = "-";
		
		if (is_dir == false)
			size = to_string(st.st_size);
		std::string space_after(20 - size.length() > 0 ? 20 - size.length() : 0, ' ');
		return space_before + this->get_date(filename) + space_after + size;
	}
	std::cerr << "webserv: [warn]: class Autoindex: information: can't find path: " << filename << "\n";
	return "";
}

std::string Autoindex::create_element(std::string name_file, bool const is_dir)
{
	std::string filename = this->format_name(name_file, is_dir);
	if (name_file == "../")
		return "<a href= \"http://" + this->current_host() + _request_path + name_file + "\">" + filename + "</a>\n";
	return "<a href= \"http://" + this->current_host() + _request_path + name_file + "\">" + filename + "</a>" + this->information(filename.length(), filename, is_dir) + "\n";
}

void Autoindex::create_list_element(std::vector<dirent*> & list, bool const is_dir)
{
	std::vector<dirent*>::iterator it = list.begin(), ite = list.end();
	std::sort(it, ite, dirent_comp);

	for (; it != ite; ++it)
		_body += this->create_element((*it)->d_name, is_dir);
}

std::string Autoindex::current_host(void)
{
	int i = 0;
	while (_host[i] && _host[i] != ':')
		++i;
	if (_host[i] == ':')
		return _host;
	return _host + ":" + to_string(_port);
}

std::string Autoindex::format_name(std::string & name, bool const & is_dir)
{
	if (is_dir)
		name.push_back('/');
	if (name.length() > 49)
	{
		name[47] = '.';
		name[48] = '.';
		name[49] = '>';
		return (name.substr(0, 50));
	}
	return name;
}


