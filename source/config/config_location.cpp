#include <iostream>
#include "server/Server.hpp"
#include "server/Location.hpp"
#include "../prototype.hpp"


Location config_location(std::string file)
{
	Location a;
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

		// if return -- maybe
		


		// if ... cgi param

		// if ... cgi pass


		// if ... method http

	}

	return a;
}