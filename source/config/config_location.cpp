#include <iostream>
#include "server/Server.hpp"
#include "server/Location.hpp"
#include "../prototype.hpp"


Location config_location(std::string file)
{
	Location a;
	int nb_autoindex = 0;
	int i = 0;

	if (file[i] == '{')
		i++;
	while (file[i] && file[i] != '}')
	{
		if (file[i] == '#')
			i = skip_comment(file, i);
		else if (isspace(file[i]))
			i = skip_space(file, i);
		else if (file.compare(i, 4, "root") == 0) {
			i += 4;
			i = parse_root(file, i, a);
		} else if (file.compare(i, 5, "index") == 0) {
			i += 5;
			i = parse_index(file, i, a);
		} else if (file.compare(i, 9, "autoindex") == 0) {
			i += 9;
			nb_autoindex++;
			if (nb_autoindex > 1)
				throw std::invalid_argument("\"autoindex\" directive is duplicat");
			i = parse_autoindex(file, i, a);
		} else if (file.compare(i, 6, "return") == 0) {
			i += 6;
			i = parse_return(file, i, a);
		} else
			i++;
		
		// if ... cgi param

		// if ... cgi pass


		// if ... method http

	}
	// std::cout << "here: " << a.get_return().first << std::endl;
	// std::cout << "here: " << a.get_return().second << std::endl;
	return a;
}