#include "../prototype.hpp"
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA_IN struct sockaddr_in
#define SA struct sockaddr
#define BUFFERSIZE 1024


int handle_connections(int client_socket);
int accept_new_connection(int server_socket);

int config_socket(int port, int backlog = 1) {
	SA_IN server_addr;
	int server_socket;
	int opt = 1;
	
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		std::cerr << "Failed to set socket options" << std::endl;
		return -1;
	}
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(server_socket, (SA*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Failed to bind the socket" << std::endl;
		return -1;
	}
	if (listen(server_socket, backlog) == -1) {
		std::cerr << "Failed to listen" << std::endl;
		return -1;
	}
	return server_socket;
}

int create_sockets() {
	fd_set current_sockets, ready_sockets;
	int server_socket = config_socket(8080, 10);

	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	while (true) {

		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
			std::cerr << "Failed to select" << std::endl;
			return -1;
		}
		
		for (int i=0; i< FD_SETSIZE ; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				if (i == server_socket) {
					int client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
				} else {
					handle_connections(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	return 0;
}


int accept_new_connection(int server_socket) {
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	if ((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)) == -1) {
		std::cerr << "accept failed" << std::endl;
	}
	return client_socket;
}


int handle_connections(int client_socket) {
	int bytes_read;
	char buffer[BUFFERSIZE] = {0};
	int msgsize = 0;

	while ((bytes_read = read(client_socket, buffer+msgsize, 1))) {
		msgsize += bytes_read;
		if (msgsize > BUFFERSIZE-1 || buffer[msgsize-1] == '\n') 
			break;
	}
	if (bytes_read == -1) {
		std::cerr << "Failed to read the request" << std::endl;
		return -1;
	}
	buffer[msgsize-1] = 0;
	std::cout << "request: " << "\n";
	std::cout << buffer << "\n";
	write(client_socket, "Hello", 5);
	close(client_socket);
	return 0;
}


// int create_socket(std::vector<Server> &servers)
// {
// 	fd_set current_sockets, ready_sockets;



// 	return 0;
// }