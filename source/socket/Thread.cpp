#include "Thread.hpp"

Thread::Thread() {}

Thread::Thread(Request request, Server server, int client_socket)
	: request(request),
	  server(server),
	  client_socket(client_socket) {}

Thread::Thread(Thread const & copy) 
	: request(copy.request),
	  server(copy.server),
	  client_socket(copy.client_socket) {}

Thread::~Thread() {}

Thread& Thread::operator=(Thread const & copy) {
	if (this == &copy)
		return *this;
	request = copy.request;
	server = copy.server;
	client_socket = copy.client_socket;
	return *this;
}


void Thread::init(Request request, Server server, int client_socket) {
	this->request = request;
	this->server = server;
	this->client_socket = client_socket;
}



