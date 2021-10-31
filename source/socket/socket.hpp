#ifndef __SOCKET_HPP__
# define __SOCKET_HPP__

# include "request/Request.hpp"
# include "../prototype.hpp"
# include "Thread.hpp"
# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <cstdio>
# include <fstream>
# include <cstring>
# include <sys/stat.h>
# include <time.h>

# define SA_IN6	struct sockaddr_in6
# define SA_IN	struct sockaddr_in
# define SA		struct sockaddr
# define BUFFERSIZE 1024
# define BACKLOG	10

# ifndef __APPLE__
#  define __APPLE__ 0
# endif

// -------------------------------- socket

int			accept_new_connection(int server_socket);
int			create_socket_ipv4(int port, int backlog);
int			create_socket_ipv6(int port, int backlog);
std::string allow_method(Request & request);
int handle_connections(int client_socket, Server & server);

// ------------------------------- socket/request

bool		is_directory(std::string path);
void    	print_request(Request & a);
char		*read_header(int client_socket, int limit);
Request 	parse_header(std::string request);

// ------------------------------- socket/response

bool		file_exist(std::string filename);
std::string call_cgi(Request & request, std::string fastcgi, int client_socket, std::string body, std::string method, std::string path);
std::string	get_file_content(std::string filename);
std::string method_get(Request & request, Server const & server, int client_socket);
void        method_delete(Request & request, Server const & server);
void 		*method_post(void *arg);
std::string	search_root(std::string path, Server const & server);
Location	search_location(std::string path, Server const & server);
Location find_location(Request & request, Server const & server, int method);
void create_response(Request & request, Server const & server, int client_socket, int current_reading);
std::string header(Request & request);
char    *read_body(int client_socket, int limit, int buffersize);
std::string cut_filename(std::string path);
std::string cut_path(std::string path);


#endif
