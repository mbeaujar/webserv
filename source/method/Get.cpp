#include "Get.hpp"

Get::Get(Server & server, Request & request, int & client_socket) : AMethods(server, request, GET, client_socket) {}

Get::Get(Get const & src) : AMethods(src._server, src._request, GET, src._client_socket)
{
	*this = src;
}

Get::~Get() {}

Get & Get::operator=(Get const & rhs)
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

void Get::execute(void)
{
	if (this->search_location() == EXIT_SUCCESS)
	{
		if (this->is_method_allowed() == true)
		{
			std::string path_cgi;

			if (is_directory(_path_file) == true && _location.get_autoindex() == true)
			{
				Autoindex a(_path_file, _request.get_path(), _request.get_host(), this->get_port());
				_body = a.get_content();
			}
			else if ((path_cgi = _location.find_path_cgi(extension(_path_file))) != "")
			{
				Cgi a(path_cgi);
				_body = a.execute(_request, GET, _client_socket, _path_file);
			}
			else
			{
				_body = get_file_content(_path_file);
			}
		}
	}
}
