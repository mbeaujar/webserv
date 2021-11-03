#include "../socket.hpp"

// Status: 200 OK
// Content-Type: text/html; charset=utf-8

int skip_line(std::string line, int i) {
	while (line[i] && line[i] != '\n')
		i++;
	return i;
}

void parse_status(Request & request, std::string error) {
	int code, i = 0;
	std::string msg;

	while (error[i] == ' ')
		i++;
	code = recup_nb(error, i);
	while (error[i] == ' ')
		i++;
	int skip = skip_line(error, i);
	msg = error.substr(i, skip - i);
	std::cout << "DEBUG: " << msg << std::endl;
	request.set_error(std::make_pair(code, msg));
}

std::string parse_cgi(Request & request, std::string response) {
	std::string error;
	int i = 0;

	if (response.compare(0, 8, "Status: ") == 0) {
		int skip = skip_line(response, 8);
		error = response.substr(8, skip);
		i = skip;
		parse_status(request, error);
		if (response[i] == '\n')
			i++;
	}
	if (response.compare(i, 14, "Content-Type: ") == 0) {
		i += 14;
		int skip = skip_line(response, i);
		error = response.substr(i, skip - i);
		i = skip;
		request.set_content_type(error);
		if (response[i] == '\n')
			i++;
	}
	return response.substr(i, response.length() - i);
}