#include <iostream>
#include "Server.hpp"
#include "../../prototype.hpp"

/**
 * @brief recup the path of the location
 * 
 * @param file server (config file)
 * @param i index
 * @return std::string path
 */
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

/**
 * @brief parse the content of the server (config file)
 * 
 * @param file bracket of the server (config file)
 * @return Server 
 */
Server config_server(std::string file) 
{
	Server a;
	int i = 0;
	int nb_client_size = 0;

	if (file[i] == '{')
		i++;
	while (file[i] && file[i] != '}')
	{
		i = skip_comment(file, i);
		i = skip_space(file, i);
		if (file.compare(i, 6, "listen") == 0) {
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
			int skip = skip_bracket(file, i);
			std::string location = file.substr(i, skip - (i - 1));
			a.adding_location(path, parse_location(location));
			i = skip + 1;
		} else if (file[i] && !isspace(file[i]) && file[i] != '#' && file[i] != '}')
			throw std::invalid_argument("unknow directive \"" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	}
 	return a;
}

/**
 * @brief split servers and parse them
 * 
 * @param file content of the file in argument
 * @return std::vector<Server> list of Server
 */
std::vector<Server> parse_server(std::string file)
{
	std::vector<Server> lst;
	size_t size = file.size();

	for (size_t i = 0; file[i]; i++)
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);	
		if (!file[i] || i > size)
			break;
		if (file[i] && i + 6 < size && file.compare(i, 6, "server") == 0) 
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
				size_t skip = skip_bracket(file, i);
				std::string server = file.substr(i, skip - (i - 1));
				lst.push_back(config_server(server));
				i = skip;
			}
			else
				throw std::invalid_argument("unknow directive, expected '{' after \"server\"");
		}
		else if (file[i] && !isspace(file[i]) && file[i] != '#') {
			if (file[i] == '}' || file[i] == '{')
				throw std::invalid_argument("unexpected \"{\" or \"}\"");
			throw std::invalid_argument("unknow directive between server");
		}
	}
	return lst;
}