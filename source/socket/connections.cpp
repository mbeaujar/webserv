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
int handle_connections(int client_socket, Server &a) {
	int bytes_read;
	char buffer[BUFFERSIZE] = {0};
	int msgsize = 0;

	while ((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFFERSIZE-1 || buffer[msgsize-1] == '\n') 
			break;
	}
	if (bytes_read == -1) {
		std::cout << buffer << std::endl;
		std::cerr << "Failed to read the request" << std::endl;
		close(client_socket);
		return -1;
	}
	buffer[msgsize-1] = 0;

	std::cout << "port: " << a.get_port()[0].port << std::endl;
	// la
	
	// print request
	std::cout << "request: " << "\n";
	std::cout << buffer << "\n";

	// send 404 error page
	write(client_socket, "HTTP/1.1 200 OK\n", 16);
	write(client_socket, "\n", 1);
	write(client_socket, "<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body bgcolor=\"white\">\n<center>\n<h1>404 Not Found</h1>\n</center>\n<hr>\n<center>webserv/1.0.0 (Ubuntu)</center>\n</body>\n</html>",176);
	close(client_socket);
	return 0;
}

// si la requete n'est pas supporté 405