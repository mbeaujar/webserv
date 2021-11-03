#include "../../prototype.hpp"
#include "../server/Location.hpp"


int is_limit(char c) {
	return (c == '{' || c == '}' || c == ';' || isspace(c) || c == '#' || c == 0);
}

int parse_return(std::string file, int i, Location &a) 
{
	int code = -1;
	int nb_word = 0;
	std::string path;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"return" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (code == -1 && isdigit(file[i])) {
			code = recup_nb(file, i);
		} else if (code != -1 && !is_limit(file[i])) {
			int skip = skip_word(file, i);
			path = file.substr(i, skip - i);
			i = skip; 
			nb_word++;
		} else if (file[i] && file[i] != ';' && !isspace(file[i]) && file[i] != '#')
			throw std::invalid_argument("pourquoi pas");
	}
	if (nb_word != 1 || code == -1)
		throw std::invalid_argument("invalid number of arguments");
	a.set_return(code, path);
	if (file[i] == ';')
		i++;
	return i;
}