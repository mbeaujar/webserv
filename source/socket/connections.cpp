#include "socket.hpp"
#include <string.h>
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
 * @brief Read the header of the request
 * 
 * @param client_socket 	fd client
 * @param limit 	client_max_body_size
 * @return char* 	content of the header
 */
char *read_header(int client_socket, int & msgsize) {
	int bytes_read;
	char *buffer;

	try {
		buffer = new char[BUFFERSIZE];
	} catch(std::exception &e) {
		std::cerr << "webserv: [warn]: read_header: " << e.what() << std::endl;
		return NULL;
	}
	memset(buffer, 0, BUFFERSIZE);
	while ((bytes_read = read(client_socket, &buffer[msgsize], 1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFFERSIZE - 1)
			break;
		if (msgsize > 4 && buffer[msgsize - 1] == '\n' && (strcmp(buffer + (msgsize - 1) - 1, "\n\n") == 0
				|| strcmp(buffer+ (msgsize - 1) - 3, "\r\n\r\n") == 0)){ // stop at blank line
			break;
		}
	}
	buffer[msgsize] = 0; // a test
	return buffer;
}

/**
 * @brief Respond error 500 to the client
 * 
 * @param r  request
 * @param server server
 * @param client_socket fd client 
 * @return int  EXIT_FAILURE
 */
int connections_error(Request & r, Server & server, int client_socket) {
	if (r.get_error().first == 200)
		r.set_error(std::make_pair(500, "Internal Server Error"));
	send_response(r, "", client_socket, server);
	return EXIT_FAILURE;
}

/**
 * @brief read and respond the client request
 * 
 * @param client_socket 
 * @return int
 */
int handle_connections(int client_socket, Server & server, std::vector<pthread_t> & threads) {
	int current_reading = 0;
	char *buffer;
	Request r;

	buffer = read_header(client_socket, current_reading);
	std::cerr << "REQUEST: " << std::endl << buffer << std::endl;
	if (buffer == NULL) 
		return connections_error(r, server, client_socket);

	if (strlen(buffer) == 0) {
		std::cerr << "Error: empty header in the request" << std::endl;
		delete [] buffer;
		return connections_error(r, server, client_socket);
	}

	if (current_reading > BUFFERSIZE - 1) {
		r.set_error(std::make_pair(413, "Request Entity Too Large"));
		delete [] buffer;
		return connections_error(r, server, client_socket);
	}
	try {
		r = parse_header(buffer);
	} catch (std::exception &e) {
		std::cerr << "webserv: [warn]: handle_connections: " << e.what() << std::endl;
 		delete [] buffer;
		return connections_error(r, server, client_socket);
	}
 	delete [] buffer;

		
	try {
		create_response(r, server, client_socket, current_reading, threads);
	} catch (std::exception &e) {
		std::cerr << "webserv: [warn]: " << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}


