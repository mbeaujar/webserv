#include "Delete.hpp"

Delete::Delete() {}

Delete::Delete(Delete const & src) {
    *this = src;
}

Delete::~Delete() {}

Delete & Delete::operator=(Delete const & rhs) {
    return *this;
}

std::string Delete::execution(Server const & server, Request & request, int & client_socket)
{
	if (file_exist(this->_path_file))
		remove_file(this->_path_file.c_str());
	else
		request.set_error(std::make_pair(404, "Not Found"));
    return "";
}
