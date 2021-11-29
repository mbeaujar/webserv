#ifndef _SOCKET_HPP_
# define _SOCKET_HPP_

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

# define ISERROR(x) (x >= 400) // HTTP code > 400 is a error
# define ISREDIRECT(x) (x != -1) // No redirection is -1

# ifndef __APPLE__
#  define __APPLE__ 0
# endif

// -------------------------------- socket

// --------- connections.cpp

int accept_new_connection(int server_socket);
int handle_connections(int client_socket, Server &server, std::vector<pthread_t> &threads);

// --------- create.cpp 

int create_socket_ipv4(int port, int backlog);
int create_socket_ipv6(int port, int backlog);


// -------------------------------- socket/request


// --------- parser.cpp

Request parse_header(std::string request);
void lower_file(std::string &request);
void print_request(Request &a);

// -------------------------------- socket/response


// --------- autoindex.cpp

std::string 	autoindex_on(std::string const &path, std::string const &root, std::string const &host, int const &port);

// --------- call_cgi.cpp

std::string 	call_cgi(Request &request, int client_socket, std::string path_to_file, std::string method, std::string path_to_cgi);

// --------- create.cpp

void 			send_response(Request & request, std::string body, int client_socket, Server const & server);
std::string 	create_error(std::string status, Server const &server, int &error);
void 			create_response(Request &request, Server const &server, int client_socket);

// --------- header.cpp

std::string 	header(Request &request);

// --------- delete.cpp

void 			method_delete(Request &request, Server const &server, int client_socket);

// --------- get.cpp

std::string path_to_file(Request &request, Location &location, int method);
int 			method_get(Server const & server, Request & request, int client_socket);

// -------- parse_cgi.cpp

std::string 	parse_cgi(Request &request, std::string response);

// --------- post.cpp

int 			method_post(Server const & server, Request & request, int client_socket);

// --------- utils.cpp

bool file_exist(std::string filename);
bool is_directory(std::string path);
int set_file_content(std::string & filename, std::string &content);
std::string get_file_content(std::string filename);
std::string cut_filename(std::string path);
std::string cut_path(std::string path);
int remove_file(char const *path);
std::string path_in_common(std::string location, std::string & path);
Location search_location(std::string path, Server const &server);
Location find_location(Request &request, Server const &server, int method);
bool is_extension(std::string & path, std::string extension);

// ---------------------- Template

/**
 * @brief close all the fds
 * 
 * @param fds list of fds
 * @return int 
 */
template <typename T>
int release_fds(std::map<int, T> &fds)
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

#endif
