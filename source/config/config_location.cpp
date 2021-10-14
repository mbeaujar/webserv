#include <iostream>
#include "server/Server.hpp"
#include "server/Location.hpp"
#include "../prototype.hpp"


Location config_location(std::string file)
{
	Location a;
	int nb_autoindex = 0;
	int i = 0;

	while (file[i])
	{
		if (file[i] == '#')
			i = skip_comment(file, i);
		else if (isspace(file[i]))
			i = skip_space(file, i);
		else
			i++;

		// if root


		// if index 
		// if autoindex
		if (file.compare(i, 9, "autoindex") == 0) {
			i += 9;
			nb_autoindex++;
			if (nb_autoindex > 1)
				throw std::invalid_argument("\"autoindex\" directive is duplicat");
			i = parse_autoindex(file, i, a);
		}

		// if return -- maybe
		


		// if ... cgi param

		// if ... cgi pass


		// if ... method http

	}

	return a;
}