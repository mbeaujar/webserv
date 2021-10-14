#include "Server.hpp"

Server::Server()
	: _port(),
	  _default_server(false),
	  _error_page(),
	  _client_size(0) {}

Server::Server(Server const &copy)
	: _port(copy._port),
	  _default_server(copy._default_server),
	  _error_page(copy._error_page),
	  _client_size(copy._client_size) {}

Server::~Server() {}

Server& Server::operator=(Server const &copy) {
	if (this == &copy)
		return *this;
	_port = copy._port;
	_default_server = copy._default_server;
	_error_page = copy._error_page;
	_client_size = copy._client_size;
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

void Server::adding_location(std::string const & path, Location const & location) {
	_location.insert(std::make_pair(path, location));
}

void Server::set_client_size(int const & client_size) {
	_client_size = client_size;
}

int Server::get_client_size() const { return _client_size; }

void Server::set_default_server(bool const & default_server) {
	_default_server = default_server;
}

bool Server::get_default_server() const { return _default_server; }

bool Server::find_port(int const & port, bool const & ipv4) const {
	std::vector<Port>::const_iterator it = _port.begin(), ite = _port.end();

	while (it != ite) {
		if (it->port == port && it->ipv4 == ipv4)
			return true;
		++it;
	}
	return false;
}

std::string Server::find_error(int const & error) const {
	std::map<int, std::string>::const_iterator search;

	search = _error_page.find(error);
	if (search != _error_page.end())
		return search->second;
	return "";
}

bool Server::find_location(std::string const & path) const {
	std::map<std::string, Location>::const_iterator it = _location.find(path);

	if (it != _location.end())
		return true;
	return false;
}

Location Server::get_location(std::string const & path) const {
	return _location.find(path)->second;
}