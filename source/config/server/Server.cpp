#include "Server.hpp"

bool Port::operator==(Port const & rhs) { return port == rhs.port && ipv4 == rhs.ipv4; }

bool Port::operator<(Port const & rhs) { // true nothing / false change
	if (port != rhs.port)
		return port < rhs.port;
	if (ipv4 == rhs.ipv4)
		return true;
	return ipv4 < rhs.ipv4;
}

Server::Server()
	: _client_size(0),
	  _default_server(false),
	  _current_port(),
	  _port(),
	  _error_page(),
	  _location() {}

Server::Server(Server const &copy)
	: _client_size(copy._client_size) ,
	  _default_server(copy._default_server),
	  _current_port(copy._current_port),
	  _port(copy._port),
	  _error_page(copy._error_page),
	  _location(copy._location) {}

Server::~Server() {}

Server& Server::operator=(Server const &copy) {
	if (this == &copy)
		return *this;
	_client_size = copy._client_size;
	_default_server = copy._default_server;
	_current_port = copy._current_port;
	_port = copy._port;
	_error_page = copy._error_page;
	_location = copy._location;
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

void Server::set_current_port(int const & port) { _current_port = port; }

int Server::get_current_port() const { return _current_port; }

int Server::get_client_size() const { return _client_size; }

void Server::set_default_server(bool const & default_server) {
	_default_server = default_server;
}

std::map<int, std::string> Server::get_error_page() const { return _error_page; }

int Server::get_number_of_port() const { return _port.size(); }

bool Server::get_default_server() const { return _default_server; }

std::vector<Port>& Server::get_port() { return _port; }

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

std::map<std::string, Location> Server::get_all_location() const { return _location; }

Location Server::get_location(std::string const & path) const {
	return _location.find(path)->second;
}