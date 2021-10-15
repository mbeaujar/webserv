#include "../../prototype.hpp"
#include "../server/Location.hpp"

int parse_root(std::string file, int i, Location &a)
{
	int count = 0;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"root" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#') 
        {
            if (file[i] && file[i] == '}')
                throw std::invalid_argument("unexpected \"}\"");
			int skip = skip_word(file, i);
			std::string word = file.substr(i, skip - i);
			a.adding_index(word);
			i = skip;
			count++;
		}
	}
	if (count == 0 || count > 1)
		throw std::invalid_argument("invalid number of arguments in \"root\" directive");
    if (file[i] && file[i] == ';')
        i++;
	return i;
}