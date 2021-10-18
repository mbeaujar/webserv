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
	}
	return i;
}
