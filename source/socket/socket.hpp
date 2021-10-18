#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

# include "../prototype.hpp"
# include <sys/select.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <cstring>

# define SA_IN struct sockaddr_in
# define SA_IN6 struct sockaddr_in6
# define SA struct sockaddr
# define BUFFERSIZE 1024
# define BACKLOG 10

int create_socket_ipv4(int port, int backlog);
int create_socket_ipv6(int port, int backlog);
int accept_new_connection(int server_socket);
int handle_connections(int client_socket);

#endif 