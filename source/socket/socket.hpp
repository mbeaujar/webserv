 #ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

# include "../prototype.hpp"
# include <sys/select.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <stdio.h>
# include <time.h>
# include <fstream>
# include <cstring>
# include "request/Request.hpp"

# define SA_IN struct sockaddr_in
# define SA_IN6 struct sockaddr_in6
# define SA struct sockaddr
# define BUFFERSIZE 1024
# define BACKLOG 10

# ifndef __APPLE__
#  define __APPLE__ 0
# endif

// -------------------------------- socket

int create_socket_ipv4(int port, int backlog);
int create_socket_ipv6(int port, int backlog);
int accept_new_connection(int server_socket);
int handle_connections(int client_socket, Server & server);

// -------------- socket/request

Request parse_request(std::string request);
void    print_request(Request const & a);
char *read_header(int client_socket, int limit);
bool is_directory(std::string path);

// -------------- socket/response

std::string search_root(std::string path, Server const & server);
bool file_exist(std::string filename);
Location search_location(std::string path, Server const & server);
std::string method_get(Request & request, Server const & server);
std::string create_response(Request & request, Server const & server);
std::string get_file_content(std::string filename);


#endif 