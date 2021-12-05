#include "AMethods.hpp"

AMethods::AMethods(Server & server, Request & request, int method, int & client_socket) :
	_method(method),
	_location(),
	_path_file(),
	_server(server),
	_request(request),
	_client_socket(client_socket),
	_body() {}


AMethods::AMethods(AMethods const & src) :
    _method(src._method),
    _location(src._location),
    _path_file(src._path_file),
	_server(src._server),
	_request(src._request),
	_client_socket(src._client_socket),
	_body(src._body) {}

AMethods::~AMethods() {}

AMethods & AMethods::operator=(AMethods const & rhs)
{
    if (this != &rhs)
    {
        _method = rhs._method;
        _location = rhs._location;
        _path_file = rhs._path_file;
		_server = rhs._server;
		_request = rhs._request;
		_client_socket = rhs._client_socket;
		_body = rhs._body;
    }
    return *this;
}

int AMethods::search_location(void)
{
	std::map<std::string, Location> list = _server.get_all_location();
	std::map<std::string, Location>::iterator it = list.begin(), ite = list.end();
	Location *tmp = NULL;

	while (it != ite)
	{
		std::string common = this->path_in_common(it->first, _request.get_path());
		if (tmp == NULL || common.length() > _path_file.length())
		{
			tmp = &it->second;
			_path_file = common;
		}
		++it;
	}
	if (tmp == NULL)
		return EXIT_FAILURE;
	_location = *tmp;
	this->update_path_file();
	return  this->path_to_file();
}

bool AMethods::is_method_allowed(void)
{
	if (_location.get_method(_method) == true)
		return true;
	_request.set_error(std::make_pair(405, "Method Not Allowed"));
	_request.set_methods(_location.get_methods());
	std::cerr << "webserv: [warn]: class AMethods: is_method_allowed: method not allowed" << "\n";
	return false;
}

std::string & AMethods::get_body(void) { return _body; } // only for GET

// ------------------------- PRIVATE ------------------------ //

std::string AMethods::path_in_common(std::string const & path_location, std::string & path)
{
	int len = 0;

	if (path_location == path)
		return path_location;
	while (path[len] && path_location[len] && path[len] == path_location[len])
		len++;
	if (path[len] != '/' && path.length() != path_location.length())
	{
		while (len > 0 && path[len] != '/')
			--len;
	}
	return path.substr(0, len);
}

void AMethods::update_path_file(void)
{
	std::string & path = _request.get_path();
	std::string & root = _location.get_root();
	int i = 0;

	while (path[i] && _path_file[i] && path[i] == _path_file[i])
		++i;
	if (path[i] && _path_file[i] && path[i] != _path_file[i])
	{
		while (i > 0 && path[i] && path[i] != '/')
			--i;
		++i;
	}
	int len = root.length();
	if (len > 1 && path[i] != '/' && root[len - 1] != '/')
		root.push_back('/'); 
	_path_file = root + path.substr(i, path.length() - i);
	_request.set_file(_path_file);
}

int AMethods::path_to_file(void)
{
	if (file_exist(_path_file))
	{
		if (is_directory(_path_file))
		{
			if (_path_file.length() > 1 && _path_file[_path_file.length() -1] != '/')
				_path_file.push_back('/');
			std::vector<std::string> index = _location.get_index();
			std::vector<std::string>::iterator it = index.begin(), ite = index.end();

			while (it != ite)
			{
				std::string path_with_index = _path_file + *it;
				if (file_exist(path_with_index) == true && is_directory(path_with_index) == false)
				{
					_path_file = path_with_index;
					return EXIT_SUCCESS;
				}
				++it;
			}
			if (_location.get_autoindex() == false && _method == GET)
			{
				std::cerr << "webserv: [warn]: class AMethods: path_to_file: Request path is a directory with autoindex off" << "\n";
				_request.set_error(std::make_pair(404, "Not Found"));
				return EXIT_FAILURE;
			}
			if (_method == POST)
			{
				if (this->path_upload() == true)
					_path_file += _location.get_index()[0];
				else
					_request.set_error(std::make_pair(400, "Bad Request"));
			}
		}
	}
	else if (_method == POST)
	{
		if (this->path_upload() == false)
		{
			_request.set_error(std::make_pair(400, "Bad Request"));
			std::cerr << "webserv: [warn]: class AMethods: path_to_file: no permission to post on this path" << "\n";
			return EXIT_FAILURE;
		}
		this->create_path();
	}
	else
	{
		_request.set_error(std::make_pair(404, "Not Found"));
		std::cerr << "webserv: [warn]: class AMethods: path_to_file: can't find path: " << _path_file << "\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

bool AMethods::path_upload(void)
{
	std::string & upload = _location.get_upload();

	if (_path_file.length() < upload.length())
		return false;
	int i = 0;
	while (upload[i] && _path_file[i] && upload[i] == _path_file[i])
		++i;
	if (upload[i])
		return false;
	return true;
}

void AMethods::create_path(void)
{
	int i = _location.get_upload().length();

	while (_path_file[i])
	{
		if (_path_file[i] == '/')
		{
			int j = 0;
			while (_path_file[j] && _path_file[j] != '/')
				j++;
			std::string word = _path_file.substr(i, j - i);
			if (_path_file[j] == '/')
			{
				if (is_directory(_path_file.substr(0, j)) == false)
						mkdir(word.c_str(), S_IRWXO);
			}
			else
			{
				int fd = open(word.c_str(), O_CREAT, S_IRWXO);
				close(fd);
			}
			i = j;
		}
		if (_path_file[i])
			i++;
	}
}


// ------------------- PROTECTED --------------------- //

bool AMethods::is_extension(std::string & path, std::string const & extension)
{
	size_t path_len = path.length();
	size_t ext_len = extension.length();

	if (extension == "" || path_len <= ext_len || extension[0] != '.')
		return false;
	std::string extension_path = path.substr(path_len - ext_len, ext_len);
	if (extension_path == extension)
		return true;
	return false;
}

int AMethods::get_port(void)
{
	std::vector<Port> list = _server.get_port();
	std::vector<Port>::iterator it = list.begin(), ite = list.end();

	while (it != ite)
	{
		if (it->fd == _client_socket)
			return it->port;
		++it;
	}
	return -1;
}

int remove_file(char const * path)
{
	int ret = remove(path);

	if (ret == -1)
		std::cerr << "webserv: [warn]: remove_file: remove fail for path: " << path << std::endl;
	return ret;
}
