#ifndef __THREAD_HPP__
# define __THREAD_HPP__

# include "request/Request.hpp"
# include "../prototype.hpp"

# include <pthread.h>
# include <sys/select.h>

struct Thread {
	Thread();
	Thread(Request request, Server server, int client_socket, int current_reading);
	Thread(Thread const &);
	~Thread();
	Thread& operator=(Thread const &);

	void init(Request request, Server server, int client_socket, int current_reading);

	Request request;
	Server server;
	int client_socket;
	int current_reading;
};

#endif