#include "Request.hpp"
#include "../socket.hpp"
#include <algorithm>

int find_query_string(std::string & request, int i)
{
	while (request[i] && request[i] != '?')
		i++;
	return i;
}

void lower_file(std::string & request)
{
	int i = 0;
	while (request[i])
	{
		if (request[i] >= 'A' && request[i] <= 'Z')
			request[i] += 32;
		if (request[i] == ':')
		{
			while (request[i] && request[i] != '\n')
				i++;
		}
		if (request[i])
			i++;
	}
}

int fuck_ali(std::string & file, int i) {
	while (file[i] && !isspace(file[i]))
		i++;
	return i;
}

// POST / HTTP/1.1
// Host: localhost:80

std::string recup_word(std::string & request, int & i)
{
	int tmp;
	std::string word;

	tmp = fuck_ali(request, i);
	word = request.substr(i, tmp - i);
	i = skip_space(request, tmp);
	return word;
}

void	get_first_line(std::string & request, Request & r)
{
	int i = 0;
	std::string word;

	word = recup_word(request, i);
	if (word == "delete")
		r.set_method(DELETE);
	else if (word == "post")
		r.set_method(POST);
	else if (word == "get")
		r.set_method(GET);
	else
		r.set_error(std::make_pair(405, "Method not allowed"));
	word = recup_word(request, i);
	r.set_path(word);
	word = recup_word(request, i);
	if (word != "http/1.1")
	{
		std::cerr << "webserv: [warn]: get_first_line: Bad HTTP version: " << word << std::endl;
		r.set_error(std::make_pair(400, "Bad request"));
	}
	word = recup_word(request, i);
	if (word != "host:")
	{
		std::cerr << "webserv: [warn]: get_first_line: Request without host part" << std::endl;
		r.set_error(std::make_pair(400, "Bad request"));
	}
	else
	{
		word = recup_word(request, i);
		r.set_host(word);
	}
}

Request parse_header(std::string request)
{
	int i = 0;
	int host = 0;
	Request r;

	lower_file(request);
	get_first_line(request, r);
	r.set_content_length(-1);
	while (request[i])
	{
		if (request.compare(i, 15, "content-length:") == 0)
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
		{
			i += 19;
			while (request[i] && request[i] != '\n')
			{
				r.add_transfer_encoding(request.substr(i, fuck_ali(request, i) - i));
				i = fuck_ali(request, i);
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
