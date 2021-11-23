#include "socket.hpp"
#include <fcntl.h>
// #include <libexplain/select.h>
#include <sys/select.h>

// /**
//  * @brief close all the fds
//  * 
//  * @param fds list of fds
//  * @return int 
//  */
// int release_fds(std::map<int, Server> & fds) {
// 	std::map<int, Server>::iterator it = fds.begin(), ite = fds.end();
// 	while (it != ite) {
// 		if (it->first != -1)
// 			close(it->first);
// 		++it;
// 	}
// 	return 0;
// }

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
			it->set_current_port(begin->port);
			sockets.insert(std::make_pair(server_socket, *it));
			++begin;
		}  
		++it;
	}
	return sockets;
}


void wait_finish(std::map<int, Server> & sockets, std::map<int, Server*> & clients, std::vector<pthread_t> & threads) {
	std::vector<pthread_t>::iterator it = threads.begin(), ite = threads.end();
	while (it != ite) {
		pthread_join(*it, NULL);
		++it;
	}
	release_fds(sockets);
	release_fds(clients);
}

/**
 * @brief Create multiple sockets who handle multiple connections
 * 
 * @param servers list of servers
 * @return int 
 */
int handle_socket(std::vector<Server> & servers) {
	fd_set current_sockets, current_clients, ready_sockets, ready_clients;
	int max_fd = 0, loop = 0;

	std::vector<pthread_t> threads;
	std::map<int, Server> sockets = config_socket(servers);
	std::map<int, Server>::iterator search;
	std::map<int, Server*>  clients;

	FD_ZERO(&current_sockets);
	FD_ZERO(&current_clients);
	std::map<int, Server>::iterator it = sockets.begin(), ite = sockets.end();
	while (it != ite) {
		if (it->first == -1) {
			release_fds(sockets);
			return EXIT_FAILURE;
		}
		fcntl(it->first, F_SETFL, O_NONBLOCK);
		if (it->first > max_fd)
			max_fd = it->first;
		FD_SET(it->first, &current_sockets);
		++it;
	}
	while (true) { 
		ready_sockets = current_sockets;
		ready_clients = current_clients;
		int fds = 0;
		if ((fds = select(max_fd + 1, &ready_sockets, &ready_clients, NULL, NULL)) < 0) // max_fd 1024 
			std::cerr << "webserv: [warn]: handle_socket: Failed to select: " << strerror(errno) << std::endl;
		if (fds > 0) {
			try {
				for (int i = 0; i < max_fd + 1; i++) {
					if (FD_ISSET(i, &ready_sockets)) {
						search = sockets.find(i);
						if (search != ite) {
							int client_socket = accept_new_connection(i);
							if (client_socket != -1) {
								fcntl(client_socket, F_SETFL, O_NONBLOCK);
								if (client_socket > max_fd)
									max_fd = client_socket;
								clients.insert(std::make_pair(client_socket, &search->second));
								FD_SET(client_socket, &current_sockets);
								FD_SET(client_socket, &current_clients);
							}
						}
						else if (FD_ISSET(i, &ready_clients)) {
							std::map<int, Server*>::iterator find = clients.find(i);
							if (find != clients.end()) {
								handle_connections(i, *find->second, threads);
								clients.erase(i);
								FD_CLR(i, &current_sockets);
								FD_CLR(i, &current_clients);
							}
							loop++;
						}
					}
				}
			} catch (std::exception &e) {
				std::cerr << "webserv: [warn]: handle_socket: " << e.what() << std::endl;
			}
		}
		if (LIMIT != -1 && loop >= LIMIT)
			break;
	}
	wait_finish(sockets, clients, threads);
	return EXIT_SUCCESS;
}
//  siege localhost -r100 -c100