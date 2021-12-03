#include "../socket.hpp"

bool file_exist(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	if (file)
	{
		file.close();
		return true;
	}
	file.close();
	return false;
}

bool is_directory(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return true;
	}
	return false;
}

int set_file_content(std::string & filename, std::string &content)
{
	std::ofstream file(filename.c_str(), std::ios_base::app);

	if (file.is_open() == false)
		return EXIT_FAILURE;
	file << content;
	file.close();
	return EXIT_SUCCESS;
}

std::string get_file_content(std::string filename)
{
	std::string content, line;
	std::ifstream file;

	file.open(filename.c_str());
	if (file.is_open() == false)
	{
		std::cerr << "Error: can't open the file: " << filename << std::endl;
		return "";
	}
	std::ostringstream contents;
	contents << file.rdbuf();
	file.close();
	return (contents.str());
}

std::string cut_filename(std::string path)
{
	int i = path.length();
	while (i > 0 && path[i] != '/')
		i--;
	std::string filename = path.substr(i + 1, path.length() - (i + 1));
	if (filename.empty() == true)
		return "/";
	return filename;
}

std::string cut_path(std::string path)
{
	int i = path.length();
	while (i > 0 && path[i] != '/')
		i--;
	return path.substr(0, i);
}

int remove_file(char const * path)
{
	int ret = remove(path);
	if (ret == -1)
		std::cerr << "webserv: [warn]: remove_file: remove fail for path: " << path << std::endl;
	return ret;
}

std::string path_in_common(std::string location, std::string & path)
{
	int len = 0;

	if (location == path)
		return location;
	while (path[len] && location[len] && path[len] == location[len])
		len++;
	if (path[len] != '/' && path.length() != location.length())
	{
		while (len > 0 && path[len] != '/')
			--len;
	}
	return path.substr(0, len);
}

std::pair<Location, std::string> search_location(std::string path, Server const &server)
{
	std::map<std::string, Location> all = server.get_all_location();
	std::map<std::string, Location>::iterator it = all.begin(), ite;
	std::pair<Location, std::string> tmp;
	tmp.first.set_return(1, "fuck le a-word");
	tmp.second = "/";
	 
	if ((ite = all.find("/")) != all.end())
	{
		tmp.first = ite->second;
		ite = all.end();
	}
	while (it != ite)
	{
		std::string common = path_in_common(it->first, path);
		if (is_extension(path, it->first))
		{
			tmp.first = it->second;
			tmp.second = common;
			return tmp;
		}
		if (common.length() > tmp.second.length())
		{
			tmp.first = it->second;
			tmp.second = common;
		}
		++it;
	}
	return tmp;
}

// root = loc root
// request = req path
// common = _path_file

std::string export_new_path(std::string & root, std::string & request, std::string & common)
{
	int i = 0;

	while (request[i] && common[i] && request[i] == common[i])
		++i;
	if (request[i] && common[i] && request[i] != common[i])
	{
		while (i > 0 && request[i] && request[i] != '/')
			--i;
		++i;
	}
	int len = root.length();
	if (len > 1 && request[i] != '/' && root[len - 1] != '/')
		root.push_back('/'); 
	return root + request.substr(i, request.length() - i);
}

// /directory/index.html - path -> /index.html

// /var/www/html/YoupiBanane - root // root + path -> /var/www/html/YoupiBanane/index.html

// /directory/ - pair.second

Location find_location(Request & request, Server const & server, int method)
{
	std::pair<Location, std::string> pair;

	if ((pair = search_location(request.get_path(), server)).first.get_return().first == 1)
	{
		std::cerr << "Error: Can't find a location for the path" << std::endl;
		request.set_error(std::make_pair(404, "Not Found"));
		return pair.first;
	}

	
	if (pair.first.get_method(method) == false)
	{
		struct s_method m = pair.first.get_methods();
		request.set_methods(m);
		request.set_error(std::make_pair(405, "Method Not Allowed"));
		std::cerr << "webserv: [warn]: find_location: Method not allowed" << std::endl;
		return pair.first;
	}
	std::string new_path = export_new_path(pair.first.get_root(), request.get_path(), pair.second); // root + path
	request.set_file(new_path);
	return pair.first;
}

bool is_extension(std::string & path, std::string extension)
{
	size_t path_len = path.length();
	size_t ext_len = extension.length();

	if (extension == "" || path_len <= ext_len)
		return false;
	std::string extension_path = path.substr(path_len - ext_len, ext_len);
	if (extension_path == extension)
		return true;
	return false;
}
