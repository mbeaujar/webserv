#include "../../prototype.hpp"
#include "../server/Location.hpp"

int parse_method(std::string file, int i, Location &a)
{
	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"return" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
        if (file[i] && file[i] != '#' && file[i] != ';') {
            if (file.substr(i, skip_word_exception(file, i) - i) == "DELETE")
                a.adding_method(DELETE);
            else if (file.substr(i, skip_word_exception(file, i) - i) == "GET")
                a.adding_method(GET);
            else if (file.substr(i, skip_word_exception(file, i) - i) == "POST")
                a.adding_method(POST);
            else
                throw std::invalid_argument("invalid method -- (je connai pas le vrai code d'erreur)");
            i = skip_word(file, i);
		}
	}
	return i;
}
