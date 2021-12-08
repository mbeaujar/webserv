#include "Thread.hpp"

Thread::Thread() {}

Thread::Thread(Server server, int client_socket, int port)
	: server(server), client_socket(client_socket), port(port) {}

Thread::Thread(Thread const &copy)
	: server(copy.server), client_socket(copy.client_socket), port(copy.port) {}

Thread::~Thread() {}

Thread& Thread::operator=(Thread const & copy)
{
	if (this != &copy)
	{
		server = copy.server;
		client_socket = copy.client_socket;
		port = copy.port;
	}
	return *this;
}

void Thread::init(Server server, int client_socket, int port)
{
	this->server = server;
	this->client_socket = client_socket;
	this->port = port;
}
