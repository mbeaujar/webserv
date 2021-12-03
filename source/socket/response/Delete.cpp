#include "Delete.hpp"

Delete::Delete(Server & server, Request & request, int & client_socket) : AMethods(server, request, DELETE, client_socket) {}

Delete::Delete(Delete const & src) : AMethods(src._server, src._request, DELETE, src._client_socket) {}

Delete::~Delete() {}

Delete & Delete::operator=(Delete const & rhs)
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

void Delete::execute(void)
{
	if (this->search_location() == EXIT_SUCCESS)
	{
		if (this->is_method_allowed() == true)
		{
			if (file_exist(this->_path_file))
				remove_file(this->_path_file.c_str());
			else
				_request.set_error(std::make_pair(404, "Not Found"));
		}
	}
}
