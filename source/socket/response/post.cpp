#include "../socket.hpp"

// response body ? : https://stackoverflow.com/questions/1226810/is-http-post-request-allowed-to-send-back-a-response-body

// RFC post : https://datatracker.ietf.org/doc/html/rfc7231#section-4.3.3

// multipart/form-data

int	read_body(int client_socket, int client_max_body_size, int file_fd, int content_length) {
	int     msgsize;
	char    *buffer;
	
	try  {
		buffer = new char[BUFFERSIZE];
	} catch (std::bad_alloc) {
		std::cerr << "Error: bad_alloc in read_body" << std::endl;
		return 1;
	}
	if (client_max_body_size == 0)
		client_max_body_size = content_length;

	for (int total_size = 0; total_size < content_length && total_size < client_max_body_size; total_size += msgsize) {	
		msgsize = 0;
		memset(buffer, 0, BUFFERSIZE);
		if (total_size + BUFFERSIZE <= content_length && total_size + BUFFERSIZE <= client_max_body_size) {
			msgsize = read(client_socket, buffer, BUFFERSIZE);
			std::cerr << "msgsize : " << msgsize << " read 1" << std::endl;
		} else {

			if (total_size + BUFFERSIZE <= content_length) { 					// si on depasse client max
				int size = client_max_body_size - total_size;
	
				msgsize = read(client_socket, buffer, size);
				std::cerr << "msgsize : " << msgsize << " read 2" << std::endl;
			}
			else {
				if (total_size + BUFFERSIZE <= client_max_body_size) { 			// si on depasse content length
					int size = content_length - total_size;
	
					msgsize = read(client_socket, buffer, size);
					std::cerr << "msgsize : " << msgsize << " read 3" << std::endl;
				}
				else {
					int size;													// on depasse les content-length et client max body size
					if (client_max_body_size < content_length)
						size = client_max_body_size - total_size;
					else
						size = content_length - total_size;
	
					msgsize = read(client_socket, buffer, size);
					std::cerr << strerror(errno) << std::endl;;
					std::cerr << "msgsize : " << msgsize << " read 4" << std::endl;
				}
			}
		}
		std::cerr << "msgsize : " << msgsize << std::endl;
		if (msgsize == 0)
			break ;
		if (msgsize == -1)
			msgsize = 0;
		buffer[msgsize] = 0;
		std::cerr << "buffer : " << buffer << std::endl;
		write(file_fd, buffer, msgsize);
	}
	delete [] buffer;
	return 0;
}


// read chuncked

void	*method_post(void *arg) {
	Thread *a = reinterpret_cast<Thread*>(arg);
	std::string tmp_file = ".post_" + to_string(a->client_socket), path, response;
	std::pair<int, std::string> error;
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
	path = path_to_file(a->request, location, POST);
	if (a->request.get_error().first != 200) {
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}
	if (is_directory(path)) {
		if (path[path.length() - 1] != '/')
			path.insert(path.end(), '/');
		path += location.get_index()[0];
	}

	if ((fd = open(tmp_file.c_str(), O_CREAT | O_RDWR, S_IRWXU)) == -1) {
		std::cerr << "Error: can't open file: " << tmp_file << std::endl;
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}

	if (a->request.get_content_length() >= 0) { // length exist donc body too
		std::cerr << "tout va bien" << std::endl;
		if (read_body(a->client_socket, a->server.get_client_size(), fd, a->request.get_content_length())) {
			send_response(a->request, "", a->client_socket);
			remove_file(tmp_file.c_str());
			close(fd);
			delete a;
			return NULL;
		}
	}
	else { // chunked request
		; 
	}


	if (is_cgi(path, location.get_fastcgi_ext()) == true) {
		response = call_cgi(a->request, a->client_socket, tmp_file, "POST", location.get_fastcgi());
		if (response.empty() == true)
			a->request.set_error(std::make_pair(502, "Bad Gateway"));
		else
			response = parse_cgi(a->request, response);
	}
	else {
		response = get_file_content(tmp_file);
	}
	
	remove_file(tmp_file.c_str());
	close(fd);

	std::ofstream		file;

	std::cerr << "response : " << response << std::endl;
	file.open(path);
	if (file.is_open() == false)
	{
		std::cerr << "Error: can't open this sheet : " << path << std::endl;
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}
	
	file << response;

	file.close();
	
	// send_response();
	send_response(a->request, "", a->client_socket);
	
	delete a;
	return NULL;
}
