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
		
		
		
		// else if (file.compare(i, 6, "method") == 0) { // if ... method http
        //     i += 6;
        //     i = parse_method(file, i, a);
		// } else if (file.compare(i, 13, "fastcgi_param") == 0) {
		// 	i += 13;
		// 	i = parse_fastcgi_param(file, i, a); 
		// } else if (file.compare(i, 12, "fastcgi_pass") == 0) { 
		// 	i += 12;
		// 	i = parse_fastcgi_pass(file, i, a); 
		// } else
		// 	i++;
		
		



	}
	return a;
}