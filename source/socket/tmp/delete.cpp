#include "../socket.hpp"

void method_delete(Request & request, Server const & server, int client_socket)
{
	std::string path;
	Location location;

	location = find_location(request, server, DELETE);
	if (request.get_error().first < 400)
	{
		path = request.get_path();
		if (file_exist(path))
		{
			remove_file(path.c_str());
		}
		else
		{
			request.set_error(std::make_pair(404, "Not Found"));
		}
	}
	send_response(request, "", client_socket, server);
}