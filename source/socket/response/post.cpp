#include "../socket.hpp"

char    *read_body(int client_socket, int limit, int buffersize) {
	int     bytes_read;
	int     msgsize = 0;
	char    *buffer = new char[buffersize];

	memset(buffer, 0, buffersize);
	if (limit == 0)
		limit = buffersize;
	while ((bytes_read = read(client_socket, buffer + msgsize, buffersize - msgsize - 1)) > 0) {
		msgsize += bytes_read;
        if (msgsize > buffersize - 1)
	 		break;
	}
	return buffer;
}

void *method_post(void *arg) {
	Thread *a = reinterpret_cast<Thread*>(arg);

	close(a->client_socket);
	delete a;
	return NULL;
}

// In any case, if a POST request is made with a Content-Type which 
// cannot be handled by the application, it should return a 415 status-code.