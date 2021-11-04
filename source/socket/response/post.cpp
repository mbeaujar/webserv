#include "../socket.hpp"

// https://stackoverflow.com/questions/1226810/is-http-post-request-allowed-to-send-back-a-response-body

int	read_body(int client_socket, int client_max_body_size, int file_fd, int content_length, int current_reading) {
	int     bytes_read;
	int     msgsize;
	char    *buffer;
	
	try  {
		buffer = new char[BUFFERSIZE];
	} catch (std::bad_alloc) {
		std::cerr << "Error: bad_alloc in read_body" << std::endl;
		return 1;
	}
	if (client_max_body_size == 0)
		client_max_body_size = content_length + current_reading;

	for (int total_size = 0; total_size < content_length && current_reading < content_length; total_size += msgsize) {	
		msgsize = 0;
		memset(buffer, 0, BUFFERSIZE);
		while ((bytes_read = read(client_socket, buffer + msgsize, BUFFERSIZE - msgsize - 1)) > 0) {
			msgsize += bytes_read;
			if (msgsize > BUFFERSIZE - 1 || msgsize + current_reading >= client_max_body_size || total_size + msgsize >= content_length)
				break;
		}
		buffer[msgsize] = 0;
		write(file_fd, buffer, BUFFERSIZE);
		if (bytes_read == 0)
			break;
	}
	delete [] buffer;
	return 0;
}

// multipart/form-data

// read chuncked

void	*method_post(void *arg) {
	Thread *a = reinterpret_cast<Thread*>(arg);
	std::string tmp_file = ".post_" + to_string(a->client_socket);
	Location location;
	int fd;
	
	// check la location
	location = find_location(a->request, a->server, POST);
	if (a->request.get_error().first != 200) {
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}

	// quoi faire si il y a une redirection ???

	// recup le path

	if ((fd = open(tmp_file.c_str(), O_CREAT | O_RDWR, S_IRWXU)) == -1) {
		std::cerr << "Error: can't open file: " << tmp_file << std::endl;
		close(a->client_socket);
		delete a;
		return NULL;
	}

	if (a->request.get_content_length() >= 0) { // length exist donc body too
		if (read_body(a->client_socket, a->server.get_client_size(), fd, a->request.get_content_length(), a->current_reading)) {
			close(a->client_socket);
			close(fd);
			delete a;
			return NULL;	
		}
	}
	else { // chunked request
		; 
	}


	// send_response();
	delete a;
	return NULL;
}
