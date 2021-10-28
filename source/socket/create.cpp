#include "socket.hpp"

/**
 * @brief Create a socket ipv4 object
 * 
 * @param port 	port to bind
 * @param backlog number of connections 
 * @return int fd of the object
 */
int create_socket_ipv4(int port, int backlog = 1) {
	SA_IN server_addr;
	int server_socket;
	
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	if (!__APPLE__) {
		int opt = 1;
		socklen_t optlen = sizeof(opt);
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, optlen) == -1) {
			std::cerr << "Failed to set socket options" << std::endl;
			close(server_socket);
			return -1;
		}
	}
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(server_socket, (SA*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Failed to bind the socket" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(server_socket);
		return -1;
	}
	if (listen(server_socket, backlog) == -1) {
		std::cerr << "Failed to listen" << std::endl;
		close(server_socket);
		return -1;
	}
	return server_socket;
}

/**
 * @brief Create a socket ipv6 object
 * 
 * @param port  port to bind
 * @param backlog number of connections
 * @return int fd of the object 
 */
int create_socket_ipv6(int port, int backlog = 1) {
	SA_IN6 server_addr;
	int server_socket;
	
	if ((server_socket = socket(AF_INET6, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	if (!__APPLE__) {
		int opt = 1;
		socklen_t optlen = sizeof(opt);
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, optlen) == -1) {
			std::cerr << "Failed to set socket options" << std::endl;
			close(server_socket);
			return -1;
		}
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(port);
	server_addr.sin6_addr = in6addr_any;
	if (bind(server_socket, (SA*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Failed to bind the socket" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(server_socket);
		return -1;
	}
	if (listen(server_socket, backlog) == -1) {
		std::cerr << "Failed to listen" << std::endl;
		close(server_socket);
		return -1;
	}
	return server_socket;
}

