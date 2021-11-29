#ifndef _THREAD_HPP_
# define _THREAD_HPP_

# include "request/Request.hpp"
# include "../prototype.hpp"

# include <pthread.h>
# include <sys/select.h>

class Thread
{
public:
	Thread();
	Thread(Server server, int client_socket);
	Thread(Thread const &);
	~Thread();
	Thread &operator=(Thread const &);

	void init(Server server, int client_socket);

	Server server;
	int client_socket;
};

#endif