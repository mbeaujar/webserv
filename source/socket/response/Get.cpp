#include "Get.hpp"

Get::Get() {}

Get::Get(Get const & src) {
    *this = src;
}

Get::~Get() {}

Get & Get::operator=(Get const & rhs) {
    if (this != &rhs)
    {

    }
    return *this;
}

std::string Get::execution(Server const & server, Request & request, int & client_socket)
{
	
}
