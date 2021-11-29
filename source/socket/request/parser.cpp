#include "Request.hpp"
#include "../socket.hpp"
#include <algorithm>

int find_query_string(std::string &request, int i)
{
	while (request[i] && request[i] != '?')
		i++;
	return i;
}

int first_line(std::string &request, Request &r, int i)
{
	std::string word = request.substr(i, skip_word_request(request, i) - i);
	size_t skip = find_query_string(word, 0);
	r.set_path(word.substr(0, skip));
	if (skip + 1 < word.length())
		r.set_query_string(word.substr(skip + 1, word.length() - (skip + 1)));
	while (request[i] == ' ')
		i++;
	while (request[i] && request[i] != '\n')
		r.set_version(r.get_version() + request[i]);
	if (request[i])
		i++;
	return i;
}

void lower_file(std::string &request)
{
	int i = 0;
	int start_line = 1;
	while (request[i] != '\0')
	{
		if (start_line == 1 && isupper(request[i]))
			request[i] += 32;
		if (start_line == 1 && request[i] == ' ')
			start_line = 0;
		if (request[i] == '\n')
			start_line = 1;
		i++;
	}
}

Request parse_header(std::string request)
{
	int i = 0;
	int host = 0;
	int method = 0;
	Request r;

	lower_file(request);
	r.set_content_length(-1);
	while (request[i])
	{
		if (request.compare(i, 5, "host:") == 0)
		{
			i += 6;
			r.set_host(request.substr(i, skip_word_request(request, i) - i));
			i = skip_word_request(request, i);
			if (method != 1)
			{
				std::cerr << "webserv: [warn]: parse_header: Host before method" << std::endl;
				r.set_error(std::make_pair(400, "Bad request"));
				return r;
			}
			host++;
		}
		else if (request.compare(i, 15, "content-length:") == 0)
		{
			i += 16;
			r.set_content_length(recup_nb(request, i));
		}
		else if (request.compare(i, 13, "content-Type:") == 0)
		{
			i += 14;
			int tmp = i;
			while (request[i] && request[i] != '\n')
				i++;
			r.set_content_type(request.substr(tmp, i - tmp));
		}
		else if (request.compare(i, 18, "transfer-encoding:") == 0)
		{ // CHUNKED GZIP
			i += 19;
			while (request[i] && request[i] != '\n')
			{
				r.add_transfer_encoding(request.substr(i, skip_word_request(request, i) - i));
				i = skip_word_request(request, i);
				if (request[i] != '\n')
					i++;
			}
		}
		else if (request.compare(i, 5, "date:") == 0)
		{
			i += 5;
			int tmp = i;
			while (request[i] && request[i] != '\n')
				i++;
			r.set_date(request.substr(tmp, i - tmp));
		}
		else
		{
			while (request[i] && request[i] != '\n')
				i++;
			if (request[i])
				i++;
		}
	}
	if (r.get_host().empty() || host > 1)
	{
		std::cerr << "webserv: [warn]: parse_header: none or too many Host field" << std::endl;
		std::cerr << request << std::endl;
		r.set_error(std::make_pair(400, "Bad request"));
		return r;
	}
	if (r.get_path().length() > 2048)
	{
		std::cerr << "webserv: [warn]: parse_header: path to long (more than 2048 characters)" << std::endl;
		r.set_error(std::make_pair(400, "Bad request"));
		return r;
	}
	return r;
}

void print_request(Request &a)
{

	std::cout << "----------- Request -----------" << std::endl;
	std::cout << " -> Host     : -> " << a.get_host() << std::endl;
	std::cout << " -> Type     : -> " << a.get_content_type() << std::endl;
	std::cout << " -> Path     : -> " << a.get_path() << std::endl;
	std::cout << " -> Method   : -> " << a.get_method() << std::endl;
	std::cout << " -> Length   : -> " << a.get_content_length() << std::endl;
	// std::cout << " -> Encoding : -> " << a.get_transfer_encoding() << std::endl;
	std::cout << " -> Error    : -> " << a.get_error().first << " -> " << a.get_error().second << std::endl;
	std::cout << " -> Return   : -> " << a.get_return().first << " -> " << a.get_return().second << std::endl;
	std::cout << " -> Query_string : -> " << a.get_query_string() << std::endl;
	std::cout << "-------------------------------" << std::endl;
}
