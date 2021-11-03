#include "Thread.hpp"

Thread::Thread() {}

Thread::Thread(Request request, Server server, int client_socket, int current_reading)
	: request(request),
	  server(server),
	  client_socket(client_socket),
	  current_reading(current_reading) {}

Thread::Thread(Thread const & copy) 
	: request(copy.request),
	  server(copy.server),
	  client_socket(copy.client_socket),
	  current_reading(copy.current_reading) {}

Thread::~Thread() {}

Thread& Thread::operator=(Thread const & copy) {
	if (this == &copy)
		return *this;
	request = copy.request;
	server = copy.server;
	client_socket = copy.client_socket;
	current_reading = copy.current_reading;
	return *this;
}


void Thread::init(Request request, Server server, int client_socket, int current_reading) {
	this->request = request;
	this->server = server;
	this->client_socket = client_socket;
	this->current_reading = current_reading;
}



