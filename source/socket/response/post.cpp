#include "../socket.hpp"

/**
 * @brief 
 * 
 * @param client_socket fd
 * @param limit client_max_body_size
 * @param buffersize  content-length
 * @return char* 
 */
char    *read_body(int client_socket, int limit, int buffersize) {
	int     bytes_read;
	int     msgsize = 0;
	char    *buffer = new char[buffersize];

	memset(buffer, 0, buffersize);
	if (limit == 0)
		limit = buffersize;
	while ((bytes_read = read(client_socket, buffer + msgsize, buffersize - msgsize - 1)) > 0) {
		msgsize += bytes_read;
        if (msgsize > buffersize - 1 || msgsize >= limit)
	 		break;
	}
	return buffer;
}

void	*method_post(void *arg) {
	Thread *a = reinterpret_cast<Thread*>(arg);

	close(a->client_socket);
	delete a;
	return NULL;
}
