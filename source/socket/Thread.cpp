#include "Thread.hpp"

Thread::Thread()
{}

Thread::Thread(Server server, int client_socket)
	: server(server), client_socket(client_socket)
{}

Thread::Thread(Thread const &copy)
	: server(copy.server), client_socket(copy.client_socket)
{}

Thread::~Thread()
{}

Thread& Thread::operator=(Thread const & copy)
{
	if (this == &copy)
		return *this;
	server = copy.server;
	client_socket = copy.client_socket;
	return *this;
}

void Thread::init(Server server, int client_socket)
{
	this->server = server;
	this->client_socket = client_socket;
}
