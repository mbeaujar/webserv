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
# include <iomanip>
# include <sstream>
# include <cstring>
# include <sys/stat.h>
# include <time.h>

# define SA_IN6	struct sockaddr_in6
# define SA_IN	struct sockaddr_in
# define SA		struct sockaddr
# define BUFFERSIZE 8192
# define BACKLOG SOMAXCONN

# ifndef __APPLE__
#  define __APPLE__ 0
# endif

// -------------------------------- socket

int			accept_new_connection(int server_socket);
int			create_socket_ipv4(int port, int backlog);
int			create_socket_ipv6(int port, int backlog);
std::string allow_method(Request & request);
int handle_connections(int client_socket, Server & server, std::vector<pthread_t> & threads);

// ------------------------------- socket/request

void    	print_request(Request & a);
char		*read_header(int client_socket, int limit);
Request 	parse_header(std::string request);
void 		lower_file(std::string & request);

// ------------------------------- socket/response

std::string autoindex_on(std::string const & path, std::string const & root, std::string const & host, int const & port);
std::string call_cgi(Request & request, int client_socket, std::string path_to_file, std::string method, std::string path_to_cgi);
std::string	get_file_content(std::string filename);
void 		*method_get(void *arg);
void        method_delete(Request & request, Server const & server);
void 		*method_post(void *arg);
std::string	search_root(std::string path, Server const & server);
Location	search_location(std::string path, Server const & server);
Location    find_location(Request & request, Server const & server, int method);
void create_response(Request & request, Server const & server, int client_socket, int current_reading, std::vector<pthread_t> & threads);
std::string header(Request & request);
std::string cut_filename(std::string path);
std::string cut_path(std::string path);
int 		remove_file(char const *path);
bool 		is_cgi(std::string & path, std::string extension_cgi);
std::string parse_cgi(Request & request, std::string response);
void send_response(Request & request, std::string body, int client_socket);
std::string create_error(std::string status, Server const & server, int & error);
std::string path_to_file(Request & request, Location & location, int method);

// ---------------------- Template

/**
 * @brief close all the fds
 * 
 * @param fds list of fds
 * @return int 
 */
template <typename T>
int release_fds(std::map<int, T> & fds) {
	typename std::map<int, T>::iterator it = fds.begin(), ite = fds.end();
	while (it != ite) {
		if (it->first != -1)
			close(it->first);
		++it;
	}
	return 0;
}



#endif
