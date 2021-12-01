#include "../socket.hpp"

int read_buffer(int fd, char **buffer, int size)
{
	int ret = recv(fd, *buffer, size, 0);
	if (ret == -1)
		std::cerr << "werbserv: [warn]: read_buffer: can't read fd: " << fd << " with buffer of " << size << std::endl;
	return ret;
}

int read_endline(int fd)
{
	char c;

	int ret = recv(fd, &c, 1, 0);
	if (ret == -1)
	{
		std::cerr << "werbserv: [warn]: read_endline: recv can't read" << std::endl;
		return EXIT_FAILURE;
	}
	if (c == '\r')
	{
		ret = recv(fd, &c, 1, 0);
		if (ret == -1)
		{
			std::cerr << "werbserv: [warn]: read_endline: recv can't read after '\\r'" << std::endl;
			return EXIT_FAILURE;
		}
		if (c != '\n')
		{
			std::cerr << "werbserv: [warn]: read_endline: recv can't read after '\\r'" << std::endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int	read_body_child(char *buffer, int file_fd, int msgsize)
{
	if (msgsize == 0) // code d'erreur -> return 1
		return 1;
	if (msgsize == -1)
		msgsize = 0;
	buffer[msgsize] = 0;
	write(file_fd, buffer, msgsize);
	return 0;
}

int	read_chunk_child(char *buffer, int file_fd, int msgsize)
{
	if (msgsize == -1)
		msgsize = 0;
	buffer[msgsize] = 0;
	std::cerr << "BUFFER chunked: " << buffer << std::endl;
	write(file_fd, buffer, msgsize);
	return 0;
}

void	read_for(int client_socket, int client_max_body_size, int file_fd, int content_length, char *buffer, int (*f)(char*, int, int))
{
	int msgsize;
	
	for (int total_size = 0; total_size < content_length && total_size < client_max_body_size; total_size += msgsize)
	{
		msgsize = 0;
		memset(buffer, 0, BUFFERSIZE);
		if (total_size + BUFFERSIZE <= content_length && total_size + BUFFERSIZE <= client_max_body_size)
			msgsize = read_buffer(client_socket, &buffer, BUFFERSIZE);
		else
		{
			if (total_size + BUFFERSIZE <= content_length) // si avec totale_size + buffer on depasse client max
			{
				int size = client_max_body_size - total_size;

				msgsize = read_buffer(client_socket, &buffer, size);
			}
			else
			{
				if (total_size + BUFFERSIZE <= client_max_body_size) // si avec totale_size + buffer on depasse content length
				{
					int size = content_length - total_size;

					msgsize = read_buffer(client_socket, &buffer, size);
				}
				else
				{
					int size; // si avec totale_size + buffer on depasse les content-length et client max body size
					if (client_max_body_size < content_length)
						size = client_max_body_size - total_size;
					else
						size = content_length - total_size;

					msgsize = read_buffer(client_socket, &buffer, size);
				}
			}
		}
		if (f(buffer, file_fd, msgsize) == 1)
			break;
	}
}

int read_body(int client_socket, int client_max_body_size, int file_fd, int content_length)
{
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

	// si client_max depasse content alors content = client
	read_for(client_socket, client_max_body_size, file_fd, content_length, buffer, read_body_child);
	delete[] buffer;
	return EXIT_SUCCESS;
}

int read_chunked_length(int client_socket)
{
	char buff;
	int nb = 0;
	int ret = 1;

	while (ret)
	{
		ret = recv(client_socket, &buff, 1, 0);
		if (ret <= 0 || buff == '\n' || buff == '\r') {
			if (buff == '\r')
				read_endline(client_socket);
			return nb;
		}
		if ((buff >= '0' && buff <= '9') || (buff >= 'a' && buff <= 'f'))
		{
			nb *= 16;
			if (buff < 58)
				nb += (buff - 48);
			else
				nb += (buff - 87);
		}
	}
	return nb;
}

int read_body_chunked(int client_socket, int client_max_body_size, int file_fd)
{
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
		// std::string hexa_length = read_chunked_length(client_socket);
		// std::cout << "HEXA: " << hexa_length << std::endl;
		// int nb = convert_hexa(hexa_length);
		int nb = read_chunked_length(client_socket);
		std::cout << "NB: " << nb << std::endl;
		if (nb == 0)
		{
			read_endline(client_socket);
			break;
		}
		int client_max = client_max_body_size; // JE NE LE GERE PAS
		if (client_max == 0)
			client_max = nb;

		read_for(client_socket, client_max, file_fd, nb, buffer, read_chunk_child);
		read_endline(client_socket);
	}
	delete[] buffer;
	return EXIT_SUCCESS;
}