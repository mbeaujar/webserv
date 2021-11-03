#include "../socket.hpp"

void    method_delete(Request & request, Server const & server) {
	std::string path;
    Location location;

    location = find_location(request, server, DELETE);
	//
	path = location.get_root() + request.get_path();
    if (file_exist(path) && is_directory(path) == false) {
        remove_file(path.c_str());
    }
}
