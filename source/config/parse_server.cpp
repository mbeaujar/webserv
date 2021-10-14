#include <iostream>
#include "server/Server.hpp"
#include "../prototype.hpp"

std::vector<Server> parse_server(std::string file)
{
	std::vector<Server> lst;

	for (int i = 0; file[i]; i++)
	{
		i = skip_space(file, i);
		if (!file[i])
			break;
		if (file.compare(i, 6, "server") == 0) 
		{
			i += 6;
			while (file[i] && file[i] != '{') { // skip space + comment
				if (isspace(file[i]))
					i = skip_space(file, i);
				else if (file[i] == '#')
					i = skip_comment(file, i);
				else 
					throw std::invalid_argument("Invalid number of arguments in \"server\"");
			}
			if (file[i] == '{') { // add a new server
				int skip = skip_bracket(file, i);
				std::string server = file.substr(i, skip - (i - 1));
				lst.push_back(config_server(server));
				std::cout << "find server" << std::endl;
				i = skip;
			}
			else
				throw std::invalid_argument("unknow directive, expected '{' after \"server\"");
		}
		else if (file[i] == '#')
			i = skip_comment(file, i);
		else if (!isspace(file[i]))
			throw std::invalid_argument("unknow directive between server");
	}
	return lst;
}