#ifndef _SOCKET_HPP_
# define _SOCKET_HPP_

# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Thread.hpp"
# include "prototype.hpp"

# include <iostream>
# include <sys/select.h>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <csignal>

# define SA_IN6	struct sockaddr_in6
# define SA_IN	struct sockaddr_in
# define SA		struct sockaddr
# define BUFFERSIZE 1024
# define BACKLOG SOMAXCONN

extern bool g_exit;

void signalHandler(int signum);
void signalPipeHandler(int signum);

class Socket
{
	public:
		Socket();
		Socket(Socket const &);
		~Socket();
		Socket & operator=(Socket const &);

		void execute(std::vector<Server> &servers);

	private:
		int 	_max_fd;
		fd_set _current_sockets;
		fd_set _current_clients;
		fd_set _ready_sockets;
		fd_set _ready_clients;
		std::vector<pthread_t> 	_threads_id;
		std::map<int, Server> 	_sockets;
		std::map<int, Server*> 	_clients;

		template <typename T>
		int release_fds(std::map<int, T> & fds)
		{
			typename std::map<int, T>::iterator it = fds.begin(), ite = fds.end();

			while (it != ite)
			{
				if (it->first != -1)
					close(it->first);
				++it;
			}
			return EXIT_SUCCESS;
		}

		int config_sockets(std::vector<Server> & servers);
		int create_socket_ipv4(int & port, int backlog);
		int create_socket_ipv6(int & port, int backlog);
		int accept_connection(int server_socket);
		int create_thread(int & client_socket, Server & server);
		int doublon_ports(std::vector<Server> & servers);
};

#endif /* _SOCKET_HPP_ */