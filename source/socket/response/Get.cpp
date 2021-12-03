#include "Get.hpp"

Get::Get(Server & server, Request & request, int & client_socket) : AMethods(server, request, GET, client_socket) {}

Get::Get(Get const & src) : AMethods(src._server, src._request, GET, src._client_socket) {}

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
    }
    return *this;
}

void Get::execute(void)
{
	if (this->search_location() == EXIT_SUCCESS)
	{
		if (this->is_method_allowed() == true)
		{
			if (is_directory(_path_file) == true && _location->get_autoindex() == true)
			{
				// Autoindex a;
				// body = a.execute();
			}
			else if (is_extension(_path_file, _location->get_cgi_ext()) == true)
			{
				Cgi a;
				_body = a.execute(_request, GET, _client_socket, _path_file);
			}
			else
			{
				_body = get_file_content(_path_file);
			}
		}
	}
}
