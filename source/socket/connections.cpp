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
char *read_header(int client_socket, int limit, int & msgsize) {
	int bytes_read;
	char *buffer;

	try {
		buffer = new char[BUFFERSIZE];
	} catch(std::bad_alloc) {
		std::cerr << "Error: bad_alloc in read_header" << std::endl;
		return NULL;
	}
	memset(buffer, 0, BUFFERSIZE);
	if (limit == 0)
		limit = BUFFERSIZE;
	while ((bytes_read = read(client_socket, &buffer[msgsize], 1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFFERSIZE - 1 || msgsize > limit - 1)
			break;
		if (msgsize > 4 && buffer[msgsize - 1] == '\n' && (strcmp(buffer + (msgsize - 1) - 1, "\n\n") == 0
				|| strcmp(buffer+ (msgsize - 1) - 3, "\r\n\r\n") == 0)){ // stop at blank line
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
int handle_connections(int client_socket, Server & server, std::vector<pthread_t> & threads) {
	int current_reading = 0;
	char *buffer = read_header(client_socket, server.get_client_size(), current_reading);
	std::cerr << "REQUEST: " << std::endl << buffer << std::endl;
	if (buffer == NULL)
		return 1;
	if (strlen(buffer) == 0) {
		delete [] buffer;
		return 1;
	}
	Request r = parse_header(buffer);
 	delete [] buffer;
	if (current_reading > BUFFERSIZE - 1)
		r.set_error(std::make_pair(413, "Request Entity Too Large"));
	create_response(r, server, client_socket, current_reading, threads);
	return 0;
}

// si la requete n'est pas supporté 405
// si pas de index ou ne trouve pas le fichier -> 403 Forbidden
// si pas de root -> chemin par default /var/wwww/html
// POST chuncked -> curl -v -d 'salut le monde' -H "Transfer-Encoding: chunked" -X POST localhost:80

