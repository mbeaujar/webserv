#include "socket.hpp"

/**
 * @brief accept a new connection (wait until there is a new connection)
 * 
 * @param server_socket 
 * @return int 
 */
int accept_new_connection(int server_socket) {
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	if ((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)) == -1) {
		std::cerr << "accept failed" << std::endl;
	}
	return client_socket;
}


/**
 * @brief read and respond the client request
 * 
 * @param client_socket 
 * @return int 
 */
int handle_connections(int client_socket) {
	int bytes_read;
	char buffer[BUFFERSIZE] = {0};
	int msgsize = 0;

	while ((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFFERSIZE-1 || buffer[msgsize-1] == '\n') 
			break;
	}
	if (bytes_read == -1) {
		std::cerr << "Failed to read the request" << std::endl;
		close(client_socket);
		return -1;
	}
	buffer[msgsize-1] = 0;
	std::cout << "request: " << "\n";
	std::cout << buffer << "\n";
	write(client_socket, "Hello", 5);
	close(client_socket);
	return 0;
}