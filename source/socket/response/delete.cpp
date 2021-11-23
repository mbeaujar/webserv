#include "../socket.hpp"

void    method_delete(Request & request, Server const & server, int client_socket) {
	std::string path;
    Location location;

    location = find_location(request, server, DELETE);
	if (request.get_error().first == 200) {
		path = request.get_path();
		if (file_exist(path)) {
			remove_file(path.c_str());
		} else {
			// quoi faire ????
		}
	}
	send_response(request, "", client_socket, server);
}