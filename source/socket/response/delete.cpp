#include "../socket.hpp"

void    method_delete(Request & request, Server const & server) {
	std::string path;
    Location location;

    location = find_location(request, server, DELETE);
	path = request.get_path();
    if (file_exist(path)) {
        remove_file(path.c_str());
    }
}