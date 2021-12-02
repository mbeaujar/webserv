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
		if (request[i] == ':' || request[i] == ' ')
		{
			while (request[i] && request[i] != '\n')
				i++;
		}
		if (request[i])
			i++;
	}
}

int skip_the_word(std::string & file, int i) {
	while (file[i] && !isspace(file[i]))
		i++;
	return i;
}

std::string recup_word(std::string & request, int & i)
{
	int tmp;
	std::string word;

	tmp = skip_the_word(request, i);
	word = request.substr(i, tmp - i);
	i = skip_space(request, tmp);
	return word;
}

void	get_first_line(std::string & request, Request & r, Server const & server)
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
	{
		int j = i;
		std::pair<Location, std::string> a = search_location(recup_word(request, j), server);
		s_method tmp = a.first.get_methods();
		r.set_methods(tmp);
		r.set_error(std::make_pair(405, "Method not allowed"));
	}
	word = recup_word(request, i);
	if (word.length() > 1 && word[word.length() - 1] == '/')
		word.erase(--word.end());
	r.set_path(word);
	word = recup_word(request, i);
	if (word != "HTTP/1.1")
	{
		std::cerr << "webserv: [warn]: get_first_line: Bad HTTP version: " << word << std::endl;
		r.set_error(std::make_pair(505, "HTTP Version Not Supporteds"));
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

Request parse_header(std::string request, Server const & server)
{
	int i = 0;
	int host = 0;
	int chunked = 0;
	Request r;

	lower_file(request);
	get_first_line(request, r, server);
	while (request[i])
	{
		if (request.compare(i, 15, "content-length:") == 0)
		{
			i += 16;
			int nb = recup_nb(request, i);
			r.set_content_length(nb);
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
				std::string word = request.substr(i, skip_the_word(request, i) - i);
				if (word != "chunked")
				{
					chunked = 1;
					r.set_error(std::make_pair(501, "Not Implemented"));
				}
				i = skip_the_word(request, i);
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
	if (r.get_content_length() == -1 && chunked == 0)
	{
		std::cerr << "webserv: [warn]: parse_header: request without content-length and chunked" << std::endl;
		r.set_error(std::make_pair(411, "Length Required"));
		return r;
	}
	if (r.get_host().empty() || host > 1)
	{
		std::cerr << "webserv: [warn]: parse_header: none or too many Host field" << std::endl;
		// std::cerr << request << std::endl;
		r.set_error(std::make_pair(400, "Bad request"));
		return r;
	}
	if (r.get_path().length() > 2048)
	{
		std::cerr << "webserv: [warn]: parse_header: path to long (more than 2048 characters)" << std::endl;
		r.set_error(std::make_pair(400, "Bad request"));
		return r;
	}
	// print_request(r);
	return r;
}

