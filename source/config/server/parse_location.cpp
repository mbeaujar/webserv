#include <iostream>
#include "Server.hpp"
#include "Location.hpp"
#include "../../prototype.hpp"


/**
 * @brief parse the content of the location (config file)
 * 
 * @param file bracket of the location
 * @return Location 
 */
Location parse_location(std::string file)
{
	int i = 0;
	Location a;
	int nb_autoindex = 0;
	int nb_client_size = 0;

	if (file[i] == '{')
		++i;
	while (file[i] && file[i] != '}')
	{
		i = skip_comment(file, i);
		i = skip_space(file, i);
		if (file.compare(i, 4, "root") == 0) {
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
		}
		else if (file.compare(i, 20, "client_max_body_size") == 0) {
		 	i += 20;
		 	nb_client_size++;
		 	if (nb_client_size > 1)
		 		throw std::invalid_argument("\"client_max_body_size\" directive is duplicate");
		 	i = parse_client_size(file, i, a);
		} else if (file.compare(i, 6, "method") == 0) {
            i += 6;
            i = parse_method(file, i, a);
        } else if (file.compare(i, 3, "cgi") == 0) {
		 	i += 3;
		 	i = parse_cgi(file, i, a);
		}
		else if (file.compare(i, 6, "upload") == 0) {
			i += 6;
			i = parse_upload(file, i ,a);
		} else if (file[i] && !isspace(file[i]) && file[i] != '#' && file[i] != '}')
			throw std::invalid_argument("unknow directive \"" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	}
    if (!a.get_method(DELETE) && !a.get_method(GET) && !a.get_method(POST)) {
        a.adding_method(DELETE);
        a.adding_method(GET);
        a.adding_method(POST);
    }
	return a;
}