#include "socket.hpp"
#include <fcntl.h>

/**
 * @brief close all the fds
 * 
 * @param fds list of fds
 * @return int 
 */
int release_fds(std::map<int, Server> & fds) {
	std::map<int, Server>::iterator it = fds.begin(), ite = fds.end();
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
 * @return std::map<int, Server>  booleen is useless
 */
std::map<int, Server> config_socket(std::vector<Server> & servers) {
	std::map<int, Server> sockets;
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
			sockets.insert(std::make_pair(server_socket, *it));
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
	std::map<int, Server> sockets = config_socket(servers);
	int max_fd = 0;

	FD_ZERO(&current_sockets);
	std::map<int, Server>::iterator it = sockets.begin(), ite = sockets.end();
	while (it != ite) {
		fcntl(it->first, F_SETFL, O_NONBLOCK); // idk
		if (it->first > max_fd)
			max_fd = it->first;
		FD_SET(it->first, &current_sockets);
		++it;
	}
	std::map<int, Server>::iterator search;
	std::map<int, Server>  clients;
	// ctrl-C pour quitter -> leaks
	while (true) {
		ready_sockets = current_sockets;
		if (select(max_fd + 1, &ready_sockets, NULL, NULL, NULL) < 0) {
			std::cerr << "Failed to select" << std::endl;
			release_fds(sockets);
			return 1;
		}
		for (int i=0; i < max_fd + 1; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				search = sockets.find(i);
				if (search != ite) {
					int client_socket = accept_new_connection(i);
					fcntl(client_socket, F_SETFL, O_NONBLOCK); // idk again
					if (client_socket > max_fd)
						max_fd = client_socket;
					clients.insert(std::make_pair(client_socket, search->second));
					FD_SET(client_socket, &current_sockets);
				} else {
					handle_connections(i, clients.find(i)->second);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
}
//  siege localhost -r100 -c100


#define SCRIPT_FILENAME 1