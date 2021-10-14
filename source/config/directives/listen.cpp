#include <iostream>
#include "../../prototype.hpp"
#include "../server/Server.hpp"


int recup_nb(std::string file, int & i) {
	int port = 0;

	while (isdigit(file[i])) {
		port = (port * 10) +  (file[i] - 48);
		i++;
	}
	return port;
}

bool parse_word(std::string file, int & i) {
	int skip = skip_word(file, i);
	std::string word = file.substr(i, skip - i);
	if (word != "default" && word != "default_server")
		throw std::invalid_argument("invalid parameter for listen \"" + word + "\"");
	i = skip;
	return true;
}

int parse_listen(std::string file, int i, Server &a)
{
	int port = -1;
	bool is_default = false;
	int ipv6 = 0;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"listen" + file.substr(i, skip_word(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file.compare(i, 5, "[::]:") == 0) { // ipv4 or ipv6
			i += 5;
			ipv6++;
			if (ipv6 > 1) // throw exception
				parse_word(file, i -= 5);
			if (!isdigit(file[i]))
				throw std::invalid_argument("invalid port in \"[::]:\" of the \"listen\" directive");
			port = recup_nb(file, i);
		}
		else if (port == -1 && isdigit(file[i])) // recup port
			port = recup_nb(file, i);
		else if (file[i] != '#' && file[i] != ';' && !isspace(file[i])) {
			is_default = parse_word(file, i);
		}
	}
	if (port == -1)
		throw std::invalid_argument("invalid number of arguments in \"listen\" directive");
	if (a.find_port(port, ipv6 == 0)) {
		std::string error = "a duplicate listen ";
		error += (ipv6 != 0 ? "[::]:" : "0.0.0.0:");
		error += to_string(port);
		throw std::invalid_argument(error);
	}
	a.adding_port(port, ipv6 == 0);
	if (is_default == true)
		a.set_default_server(is_default);	
	return i;
}