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
		} else {

			if (total_size + BUFFERSIZE <= content_length) { 					// si on depasse client max
				int size = client_max_body_size - total_size;
	
				msgsize = read(client_socket, buffer, size);
			}
			else {
				if (total_size + BUFFERSIZE <= client_max_body_size) { 			// si on depasse content length
					int size = content_length - total_size;
	
					msgsize = read(client_socket, buffer, size);
				}
				else {
					int size;													// on depasse les content-length et client max body size
					if (client_max_body_size < content_length)
						size = client_max_body_size - total_size;
					else
						size = content_length - total_size;
	
					msgsize = read(client_socket, buffer, size);
				}
			}
		}
		if (msgsize == 0) // code d'erreur -> return 1
			break ;
		if (msgsize == -1)
			msgsize = 0;
		buffer[msgsize] = 0;
		write(file_fd, buffer, msgsize);
	}
	delete [] buffer;
	return 0;
}

int	convert_hexa(std::string str) {

	int i = 0;
	int res = 0;
	
	while (str[i] && ((str[i] >= '1' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z')))
	{
		res *= 16;
		res += str[i] < 58 ? str[i] - 48 : str[i] - 87;
		i++;
	}
	return res;
}

std::string read_chunked_length(int client_socket) {
	std::string buffer;
	char buff;
	int ret = 1;

	while (ret) {
		ret = read(client_socket, &buff, 1);
		if (ret == 0)
			return buffer;
		if (buff == '\n')
			return buffer;
		buffer.insert(buffer.end(), buff);
	}
	return buffer;
}

// read chuncked
int read_body_chunked(int client_socket, int client_max_body_size, int file_fd)
{
	int msgsize = 0;
	char *buffer;

	try {
		buffer = new char[BUFFERSIZE];
	}
	catch (std::bad_alloc) {
		std::cerr << "Error: bad_alloc in read_body" << std::endl;
		return 1;
	}
	while (true)
	{
		// while (read(client_socket, buffer, 0) < 0)
			// ;
		std::string hexa_length = read_chunked_length(client_socket);
		int nb = convert_hexa(hexa_length);
		if (nb == 0)
			break;
		
		int client_max = client_max_body_size;
		if (client_max == 0)
			client_max = nb;
			
		for (int total_size = 0; total_size < nb; total_size += msgsize)
		{
			msgsize = 0;
			memset(buffer, 0, BUFFERSIZE);
			if (total_size + BUFFERSIZE <= nb && total_size + BUFFERSIZE <= client_max)
			{
				msgsize = read(client_socket, buffer, BUFFERSIZE);
			}
			else
			{
				if (total_size + BUFFERSIZE <= nb)
				{ // si on depasse client max
					int size = client_max - total_size;

					msgsize = read(client_socket, buffer, size);
				}
				else
				{
					if (total_size + BUFFERSIZE <= client_max)
					{ // si on depasse content length
						int size = nb - total_size;

						msgsize = read(client_socket, buffer, size);
					}
					else
					{
						int size; // on depasse les content-length et client max body size
						if (client_max < nb) {
							size = client_max - total_size;
						} else
							size = nb - total_size;
						msgsize = read(client_socket, buffer, size);
					}
				}
			}
			if (msgsize == -1)
				msgsize = 0;
			buffer[msgsize] = 0;
			write(file_fd, buffer, msgsize);
		}
		char c;
		read(client_socket, &c, 1);
		if (c != '\n')
			std::cerr << "LA GROSSE MERDE" << std::endl;
	}
	delete[] buffer;
	return 0;
}

// path : 	/var/www/html/doc/test.html

// ulpoad : /var/www/html/doc

bool	path_upload(std::string path, std::string upload) {
	if (path.length() < upload.length())
		return false;
	int i = 0;
	while (upload[i] && upload[i] == path[i])
		i++;
	if (upload[i])
		return false;
	return true;
}

void	*method_post(void *arg) {
	Thread *a = reinterpret_cast<Thread*>(arg);
	std::string tmp_file = ".post_" + to_string(a->client_socket), path, response;
	Location location;
	int fd;
	
	// check la location
	location = find_location(a->request, a->server, POST);
	if (a->request.get_error().first != 200) {
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}

	// recup le path
	path = path_to_file(a->request, location, POST);
	if (a->request.get_error().first != 200) {
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}

	// si le chemin existe pas
	if (file_exist(path) == false) {
		// on upload ??
		if (path_upload(path, location.get_upload()) == false) {
			// code erreur si on peux pas upload ??
			send_response(a->request, "", a->client_socket);
			delete a;
			return NULL;
		}
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

	if (a->request.get_content_length() >= 0) {
		if (read_body(a->client_socket, a->server.get_client_size(), fd, a->request.get_content_length())) {
			send_response(a->request, "", a->client_socket);
			remove_file(tmp_file.c_str());
			close(fd);
			delete a;
			return NULL;
		}
	}
	else { // chunked request
		if (read_body_chunked(a->client_socket, a->server.get_client_size(), fd)) {
			send_response(a->request, "", a->client_socket);
			remove_file(tmp_file.c_str());
			close(fd);
			delete a;
			return NULL;
		}
	}

	if (is_cgi(path, location.get_fastcgi_ext()) == true) {
		if (file_exist(location.get_fastcgi()) == false)
		{
			a->request.set_error(std::make_pair(502, "Bad Gateway")); // pas sur de l'hortographe
			std::cerr << "Error: no cgi man click !" << std::endl;
			send_response(a->request, "", a->client_socket);
			remove_file(tmp_file.c_str());
			close(fd);
			delete a;
			return NULL;
		}
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
	file.open(path.c_str());
	if (file.is_open() == false)
	{
		std::cerr << "Error: can't open this sheet : " << path << std::endl;
		send_response(a->request, "", a->client_socket);
		delete a;
		return NULL;
	}
	file << response;
	file.close();
	
	send_response(a->request, "", a->client_socket);

	delete a;
	return NULL;
}
