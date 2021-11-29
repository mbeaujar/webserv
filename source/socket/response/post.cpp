#include "../socket.hpp"

// response body ? : https://stackoverflow.com/questions/1226810/is-http-post-request-allowed-to-send-back-a-response-body

// RFC post : https://datatracker.ietf.org/doc/html/rfc7231#section-4.3.3

// multipart/form-data

int read_body(int client_socket, int client_max_body_size, int file_fd, int content_length)
{
	int msgsize;
	char *buffer;

	try
	{
		buffer = new char[BUFFERSIZE];
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [warn]: read_body: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	if (client_max_body_size == 0)
		client_max_body_size = content_length;

	for (int total_size = 0; total_size < content_length && total_size < client_max_body_size; total_size += msgsize)
	{
		msgsize = 0;
		memset(buffer, 0, BUFFERSIZE);
		if (total_size + BUFFERSIZE <= content_length && total_size + BUFFERSIZE <= client_max_body_size)
		{
			msgsize = read(client_socket, buffer, BUFFERSIZE);
		}
		else
		{

			if (total_size + BUFFERSIZE <= content_length)
			{ // si on depasse client max
				int size = client_max_body_size - total_size;

				msgsize = read(client_socket, buffer, size);
			}
			else
			{
				if (total_size + BUFFERSIZE <= client_max_body_size)
				{ // si on depasse content length
					int size = content_length - total_size;

					msgsize = read(client_socket, buffer, size);
				}
				else
				{
					int size; // on depasse les content-length et client max body size
					if (client_max_body_size < content_length)
						size = client_max_body_size - total_size;
					else
						size = content_length - total_size;

					msgsize = read(client_socket, buffer, size);
				}
			}
		}
		if (msgsize == 0) // code d'erreur -> return 1
			break;
		if (msgsize == -1)
			msgsize = 0;
		buffer[msgsize] = 0;
		write(file_fd, buffer, msgsize);
	}
	delete[] buffer;
	return EXIT_SUCCESS;
}

int convert_hexa(std::string str)
{

	int i = 0;
	int res = 0;

	while (str[i] && ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z')))
	{
		res *= 16;
		res += str[i] < 58 ? str[i] - 48 : str[i] - 87;
		i++;
	}
	return res;
}

std::string read_chunked_length(int client_socket)
{
	std::string buffer;
	char buff;
	int ret = 1;

	while (ret)
	{
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

	try
	{
		buffer = new char[BUFFERSIZE];
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [warn]: read_body_chunked: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	while (true)
	{
		// while (read(client_socket, buffer, 0) < 0)
		// 	;
		std::string hexa_length = read_chunked_length(client_socket);
		int nb = convert_hexa(hexa_length);
		if (nb == 0)
		{
			// probleme read
			char c = 0;
			read(client_socket, &c, 1);
			if (c == '\r')
				read(client_socket, &c, 1);
			read(client_socket, &c, 1);
			break;
		}

		int client_max = client_max_body_size; // JE NE LE GERE PAS
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
						if (client_max < nb)
						{
							size = client_max - total_size;
						}
						else
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
		if (c == '\r')
		{
			read(client_socket, &c, 1);
			if (c != '\n')
				std::cerr << "webserv: [warn]: read_body_chunked: no '\\n' at the end of the line (read header request)" << std::endl;
		}
		else if (c != '\n')
			std::cerr << "webserv: [warn]: read_body_chunked: no '\\n' at the end of the line (read header request)" << std::endl;
	}
	delete[] buffer;
	return EXIT_SUCCESS;
}

bool path_upload(std::string path, std::string upload)
{
	if (path.length() < upload.length())
		return false;
	int i = 0;
	while (upload[i] && upload[i] == path[i])
		i++;
	if (upload[i])
		return false;
	return true;
}

int parse_post(Server const & server, Request & request, int client_socket)
{
	std::string tmp_file;
	std::string response;
	std::string path;
	Location location;
	int fd;


	tmp_file = ".post_" + to_string(client_socket);
	location = find_location(request, server, POST);
	if (ISERROR(request.get_error().first))
	{
		return EXIT_FAILURE;
	}

	// recup le path
	path = path_to_file(request, location, POST);
	if (ISERROR(request.get_error().first))
	{
		return EXIT_FAILURE;
	}

	// si le chemin existe pas
	if (file_exist(path) == false)
	{
		// on upload ??
		if (path_upload(path, location.get_upload()) == false)
		{
			// code erreur si on peux pas upload ??
			return EXIT_FAILURE;
		}
	}

	if (is_directory(path))
	{
		if (path.length() > 1 && path[path.length() - 1] != '/')
			path.insert(path.end(), '/');
		std::vector<std::string> index = location.get_index();
		if (index.size() > 1)
			path += location.get_index()[0];
	}

	if ((fd = open(tmp_file.c_str(), O_CREAT | O_RDWR, S_IRWXU)) == -1)
	{
		std::cerr << "webserv: [warn]: parse_post: can't open: " << tmp_file << std::endl;
		request.set_error(std::make_pair(500, "Internal Server Error"));
		return EXIT_FAILURE;
	}
	// std::cerr << "CGI" << std::endl;
	if (request.get_content_length() >= 0)
	{
		if (read_body(client_socket, location.get_max_body(), fd, request.get_content_length()))
		{
			request.set_error(std::make_pair(500, "Internal Server Error"));
			remove_file(tmp_file.c_str());
			close(fd);
			return EXIT_FAILURE;
		}
	}
	else
	{ // chunked request
		if (read_body_chunked(client_socket, location.get_max_body(), fd))
		{
			request.set_error(std::make_pair(500, "Internal Server Error"));
			remove_file(tmp_file.c_str());
			close(fd);
			return EXIT_FAILURE;
		}
	}

	// if content-type: ...form... alors set request.set_query_string()

	if (is_extension(path, location.get_fastcgi_ext()) == true)
	{
		if (file_exist(location.get_fastcgi()) == false)
		{
			request.set_error(std::make_pair(502, "Bad Gateway"));
			remove_file(tmp_file.c_str());
			close(fd);
			return EXIT_FAILURE;
		}
		response = call_cgi(request, client_socket, tmp_file, "POST", location.get_fastcgi());
		if (response.empty() == true)
			request.set_error(std::make_pair(502, "Bad Gateway"));
		else
			response = parse_cgi(request, response);
	}
	else
	{
		response = get_file_content(tmp_file);
	}

	remove_file(tmp_file.c_str());
	close(fd);

	if (set_file_content(tmp_file, response) == EXIT_FAILURE)
	{
		std::cerr << "webserv: [warn]: parse_post: can't open: " << path << std::endl;
		request.set_error(std::make_pair(500, "Internal Server Error"));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int method_post(Server const & server, Request & request, int client_socket)
{
	try
	{
		parse_post(server, request, client_socket);
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [warn]: method_post: " << e.what() << std::endl;
		request.set_error(std::make_pair(500, "Internal Server Error"));
	}
	send_response(request, "", client_socket, server);
	return EXIT_SUCCESS;
}
