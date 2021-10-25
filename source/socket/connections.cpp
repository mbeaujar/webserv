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

char *read_header(int client_socket, int limit) {
	int bytes_read;
	char *buffer = new char[BUFFERSIZE];
	int msgsize = 0;

	memset(buffer, 0, BUFFERSIZE);
	if (limit == 0)
		limit = BUFFERSIZE;
	while ((bytes_read = read(client_socket, &buffer[msgsize], 1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFFERSIZE - 1 || msgsize > limit - 1)
			break;
		if (msgsize - 1 > 3 && buffer[msgsize - 1] == '\n' && strcmp(buffer + msgsize - 4, "\r\n\r\n") == 0) { // stop at blank line
			break;
		}
	}
	return buffer;
}

/**
 * @brief read and respond the client request
 * 
 * @param client_socket 
 * @return int
 */
int handle_connections(int client_socket, Server & server) {

	char *buffer = read_header(client_socket, server.get_client_size());
	std::cout << "request: " << std::endl;
	std::cout << buffer << std::endl;

	Request r = parse_request(buffer); // si il casse buffer n'est pas free
	delete [] buffer;

	std::string response = create_response(r, server);
	print_request(r);
	std::cout << response << std::endl;
	write(client_socket, response.c_str(), response.length());
	
	close(client_socket);
	return 0;
}

// si la requete n'est pas supporté 405
// si pas de index ou ne trouve pas le fichier -> 403 Forbidden
// si pas de root -> chemin par default /var/wwww/html
// POST chuncked -> curl -v -d 'salut le monde' -H "Transfer-Encoding: chunked" -X POST localhost:80

