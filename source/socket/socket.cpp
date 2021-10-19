#include "socket.hpp"

/**
 * @brief close all the fds
 * 
 * @param fds list of fds
 * @return int 
 */
int release_fds(std::map<int, bool> & fds) {
	std::map<int, bool>::iterator it = fds.begin(), ite = fds.end();
	while (it != ite) {
		close(it->first);
		++it;
	}
	return 0;
}


/**
 * @brief Create a map of fd (fd = servers port)
 * 
 * @param servers list of servers
 * @return std::map<int, bool>  booleen is useless
 */
std::map<int, bool> config_socket(std::vector<Server> & servers) {
	std::map<int, bool> sockets;
	std::vector<Server>::iterator it = servers.begin(), ite = servers.end();

	while (it != ite) {
		std::vector<Port> lst = it->get_port();
		std::vector<Port>::iterator begin = lst.begin(), end = lst.end();
		while (begin != end) {
			int server_socket;
			if (begin->ipv4)
				server_socket = create_socket_ipv4(begin->port, BACKLOG);
			else
				server_socket = create_socket_ipv6(begin->port, BACKLOG);
			sockets.insert(std::make_pair(server_socket, begin->ipv4));
			++begin;
		}  
		++it;
	}
	return sockets;
}

/**
 * @brief Create multiple sockets who handle multiple connections
 * 
 * @param servers list of servers
 * @return int 
 */
int handle_socket(std::vector<Server> & servers) {
	fd_set current_sockets, ready_sockets;
	std::map<int, bool> sockets = config_socket(servers);

	FD_ZERO(&current_sockets);
	std::map<int, bool>::iterator it = sockets.begin(), ite = sockets.end();
	while (it != ite) {
		FD_SET(it->first, &current_sockets);
		++it;
	}
	// ctrl-C pour quitter -> leaks
	while (true) {
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
			std::cerr << "Failed to select" << std::endl;
			release_fds(sockets);
			return 1;
		}
		for (int i=0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				if (sockets.find(i) != ite) {
					int client_socket = accept_new_connection(i);
					FD_SET(client_socket, &current_sockets);
				} else {
					handle_connections(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}

}
