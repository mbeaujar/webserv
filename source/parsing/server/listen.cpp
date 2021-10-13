#include <iostream>
#include "../../prototype.hpp"
#include "Server.hpp"


int parse_port(std::string file, int & i) {
	int port = 0;

	while (isdigit(file[i])) {
		port += (file[i] - 48);
		i++;
	}
	return port;
}

bool parse_word(std::string file, int & i) {
	int skip = skip_word(file, i);
	std::string word = file.substr(i, skip - i);
	// std::cout << "ici: " << word << std::endl;
	// std::cout << "i: " << i << std::endl;
	// std::cout << "skip: " << skip << std::endl;
	if (word != "default" && word != "default_server")
		throw std::invalid_argument("invalid parameter for listen");
	i = skip;
	return true;
}

int parse_listen(std::string file, int i, Server &a)
{
	int port = -1;
	bool is_default = false;
	int ipv6 = 0;

	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		if (file[i] == '#')
			i = skip_comment(file, i);
		if (file.compare(i, 5, "[::]:") == 0) { // ipv4 or ipv6
			i += 5;
			ipv6++;
			port = parse_port(file, i);
		}
		else if (port == -1 && isdigit(file[i])) // recup port
			port = parse_port(file, i);
		else if (file[i] != '#' && file[i] != ';' && !isspace(file[i])) {
			is_default = parse_word(file, i);
		}



		// if (file[i] && !isspace(file[i]) && file[i] != '#' && file[i] != ';') { // check default or default_server
		// 	std::cout << "je: " << file[i] << std::endl;
		// 	is_default = parse_word(file, i);
		// }
		// if (file[i] && file[i] != ';')
	}
	if (port == -1)
		throw std::invalid_argument("lalal");
	if (ipv6 > 1) {
		std::cout << "nb ipv6: " << ipv6 << std::endl;
		throw std::invalid_argument("lolol");
		
	}
	a.adding_port(port, ipv6 == 0);
	a.set_default_server(is_default);	
	return i;
}