#include <iostream>
#include "server/Server.hpp"
#include "../prototype.hpp"

// 1 root max

// 1 location avec le même chemin max 

// error_page illimité -- si il y a 2 error_page c'est le premier dans le fichier qui compte

Server create_server(std::string file) 
{
	Server a;

	for (int i = 0; file[i]; i++)
	{
		if (file[i] == '#')
			i = skip_comment(file, i);
		i = skip_space(file, i);
		// if listen
		std::cout << "i: " << i << std::endl;
		if (file.compare(i, 6, "listen") == 0) {
			i += 6;
			// i = parse_listen(file, i, a);
		}

		// if error_page

		// if limit_body_size

		// if location
	}
	std::cout << "a port: " << a.find_port(90) << std::endl;
	std::cout << "a default: " << a.get_default_server() << std::endl;
	return a;
}