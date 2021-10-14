#include <iostream>
#include "server/Server.hpp"
#include "../prototype.hpp"

// 1 root max

// 1 location avec le même chemin max 

// error_page illimité -- si il y a 2 error_page c'est le premier dans le fichier qui compte


std::string recup_path_location(std::string file, int & i) {
	std::string word = "";
	
	while (file[i] && file[i] != '{') {
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && !isspace(file[i]) && file[i] != '#' && file[i] != '{') {
			if (word != "")
				throw std::invalid_argument("invalid number of arguments in \"location\" directive");
			int skip = skip_word(file, i);
			word = file.substr(i, skip - i);
			i = skip;
		}
	}
	if (word == "")
		throw std::invalid_argument("invalid number of arguments in \"location\" directive");
	return word;
}


Server config_server(std::string file) 
{
	Server a;
	int i = 0;
	int nb_client_size = 0;

	while (file[i])
	{
		if (file[i] == '#')
			i = skip_comment(file, i);
		else if (isspace(file[i]))
			i = skip_space(file, i);
		else if (file.compare(i, 6, "listen") == 0) {
			i += 6;
			i = parse_listen(file, i, a);
		} else  if (file.compare(i, 10, "error_page") == 0) {
			i += 10;
			i = parse_error_page(file, i, a);
		} else if (file.compare(i, 20, "client_max_body_size") == 0) {
			i += 20;
			nb_client_size++;
			if (nb_client_size > 1)
				throw std::invalid_argument("\"client_max_body_size\" directive is duplicate");
			i = parse_client_size(file, i, a);
		} else if (file.compare(i, 8, "location") == 0) {
		 	i += 8;
			std::string path = recup_path_location(file, i);
			if (file[i] != '{')
				throw std::invalid_argument("invalid number of arguments in \"location\" directive");
			if (a.find_location(path))
				throw std::invalid_argument("duplicate location \"" + path + "\"");
			// recup le path
			int skip = skip_bracket(file, i);
			std::string location = file.substr(i, skip - (i - 1));
			a.adding_location(path, config_location(location));
			i = skip + 1;
		} else
			i++;

		// if location
	}
	Location tmp = a.get_location("/");
	std::cout << "autoindex: " << tmp.get_autoindex() << std::endl;
 	return a;
}
