#include "../socket.hpp"

// Status: 200 OK
// Content-Type: text/html; charset=utf-8

int header_size(std::string response)
{
	int i = 0;
	while (response[i])
	{
		if (response.compare(i, 4, "\r\n\r\n") == 0)
			return i + 4;
		if (response.compare(i, 2, "\n\n") == 0)
			return i + 2;
		i++;
	}
	return i;
}

int skip_line(std::string line, int i)
{
	while (line[i] && line[i] != '\n')
		i++;
	return i;
}

void parse_status(Request &request, std::string error)
{
	int code, i = 0;

	while (isspace(error[i]) && error[i] != '\n')
		i++;
	code = recup_nb(error, i);
	while (isspace(error[i]) && error[i] != '\n')
		i++;
	int skip = skip_line(error, i);
	request.set_error(std::make_pair(code, error.substr(i, skip - i)));
}

void parse_header(Request &request, std::string header)
{
	for (int i = 0; header[i]; i++)
	{
		if (header.compare(i, 8, "status: ") == 0)
		{
			i += 8;
			int skip = skip_line(header, i);
			parse_status(request, header.substr(i, skip - i));
			i = skip;
			if (header[i] == '\n')
				i++;
		}
		if (header.compare(i, 14, "content-type: ") == 0)
		{
			i += 14;
			int skip = skip_line(header, i);
			request.set_content_type(header.substr(i, skip - i));
			i = skip;
			if (header[i] == '\n')
				i++;
		}
		if (header.compare(i, 16, "content-length: ") == 0)
		{
			i += 16;
			int code = recup_nb(header, i);
			request.set_content_length(code);
			if (header[i] == '\n')
				i++;
		}
	}
}

std::string parse_cgi(Request &request, std::string response)
{
	int len = header_size(response);
	std::string header = response.substr(0, len);
	std::string body = response.substr(len, response.length() - len);

	lower_file(header);
	parse_header(request, header);
	return body;
}
