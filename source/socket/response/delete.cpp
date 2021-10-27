#include "../socket.hpp"

void    method_delete(Request & request, Server const & server) {
    std::string r_root;
	std::string path;
    Location location;

    if (request.get_error().first != 200)
		return "";
	if ((location = search_location(request.get_path(), server)).get_return().first == 1) {
		std::cerr << "Error: Can't find a location for the path" << std::endl;
		request.set_error(std::make_pair(404, "Not Found"));
		return "";
	}
    r_root = location.get_root();
	if (request.get_path() == "/")
		path = r_root;
	else
		path = r_root + request.get_path();
    if (file_exist(path)) {
        remove(path.c_str());
    }
}
