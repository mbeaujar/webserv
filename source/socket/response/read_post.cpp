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
	write(file_fd, buffer, msgsize);
	return 0;
}

int	read_for(int client_socket, int file_fd, int content_length, char *buffer, int (*f)(char*, int, int))
{
	int msgsize;
	int total_size;
	
	for (total_size = 0; total_size < content_length; total_size += msgsize)
	{
		msgsize = 0;
		memset(buffer, 0, BUFFERSIZE);
		if (total_size + BUFFERSIZE < content_length)
			msgsize = read_buffer(client_socket, &buffer, BUFFERSIZE);
		else
			msgsize = read_buffer(client_socket, &buffer, (content_length - total_size));	
		if (f(buffer, file_fd, msgsize) == 1)
			break ;
	}
	return total_size;
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
	if (client_max_body_size != 0 && content_length > client_max_body_size)
		content_length = client_max_body_size;
	if (content_length != (read_for(client_socket, file_fd, content_length, buffer, read_body_child)))
		std::cerr << "webserv: [warn]: read_body: read_for return" << std::endl;
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
	int total_size;
	int client_max;

	try
	{
		buffer = new char[BUFFERSIZE];
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: [warn]: read_body_chunked: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	total_size = 0;
	client_max = client_max_body_size;
	while (true)
	{
		int nb = read_chunked_length(client_socket);
		if (nb == 0)
		{
			read_endline(client_socket);
			break ;
		}
		if (client_max_body_size == 0)
			client_max = total_size + nb;
		if (total_size + nb > client_max)
			nb = client_max - total_size;
		total_size += read_for(client_socket, file_fd, nb, buffer, read_chunk_child);
		read_endline(client_socket);
		if (client_max_body_size != 0 && total_size + nb > client_max_body_size)
			break ;
	}
	delete[] buffer;
	return EXIT_SUCCESS;
}