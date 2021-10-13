#include "Server.hpp"

Server::Server()
	: _port(),
	  _default_server(false),
	  _error_page(),
	  _limit_body_size(-1) {}

Server::Server(Server const &copy)
	: _port(copy._port),
	  _default_server(copy._default_server),
	  _error_page(copy._error_page),
	  _limit_body_size(copy._limit_body_size) {}

Server::~Server() {}

Server& Server::operator=(Server const &copy) {
	if (this == &copy)
		return *this;
	_port = copy._port;
	_default_server = copy._default_server;
	_error_page = copy._error_page;
	_limit_body_size = copy._limit_body_size;
	return *this;
}

void Server::adding_port(int const &port, bool const &ipv4 = true) {
	Port a;
	a.port = port;
	a.ipv4 = ipv4;
	_port.push_back(a);
}

void Server::adding_error_page(int const &error, std::string const &path) {
	_error_page.insert(std::make_pair(error, path));
}

void Server::set_limit_body_size(int const & limit_body_size) {
	_limit_body_size = limit_body_size;
}

int Server::get_limit_body_size() const { return _limit_body_size; }

void Server::set_default_server(bool const & default_server) {
	_default_server = default_server;
}

bool Server::get_default_server() const { return _default_server; }

bool Server::find_port(int const & port) const {
	std::vector<Port>::const_iterator it = _port.begin(), ite = _port.end();

	while (it != ite) {
		if (it->port == port)
			return true;
		it++;
	}
	return false;
}