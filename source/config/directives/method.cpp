#include "prototype.hpp"

int parse_method(std::string file, int i, Location &a)
{
    std::string word;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"return" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
        if (file[i] && file[i] != '#' && file[i] != ';') {
            word = file.substr(i, skip_word_exception(file, i) - i);
            if (word == "DELETE")
                a.adding_method(DELETE);
            else if (word == "GET")
                a.adding_method(GET);
            else if (word == "POST")
                a.adding_method(POST);
            else
                throw std::invalid_argument("invalid method");
            i = skip_word(file, i);
		}
	}
	if (file[i] == ';')
		++i;
	return i;
}