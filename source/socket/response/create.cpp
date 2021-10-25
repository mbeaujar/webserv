
#include "../socket.hpp"

// #include "../request/Request.hpp"


// GET TIME -> https://stackoverflow.com/questions/7548759/generate-a-date-string-in-http-response-date-format-in-c

// std::string create_header(Request & a) {
// 	std::string header;


// }

std::string create_response(Request & request, Server const & server) {
	std::string response;
	std::string body;

	if (request.get_method() == GET) {
		body = method_get(request, server);
	}
	if (request.get_method() == POST) {
		// body = method_get(request, server);
	}
	if (request.get_method() == DELETE) {
		// body = method_get(request, server);
	}
	if (request.get_error().first != 200) {
		
	}
	// response = get_header();
	// response += "\n"; // blank line
	response += body;
	return response;
}