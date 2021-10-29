#include "../socket.hpp"

void    method_delete(Request & request, Server const & server) {
    std::string r_root;
	std::string path;
    Location location;

	
    r_root = location.get_root();
	if (request.get_path() == "/")
		path = r_root;
	else
		path = r_root + request.get_path();
    if (file_exist(path) && is_directory(path) == false) {
        remove(path.c_str());
    }
}
