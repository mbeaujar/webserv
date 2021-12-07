#include "prototype.hpp"

int	parse_autoindex(std::string file, int i, Location &a)
{
	int arg = 0;
    bool autoindex = false;
    std::string state;
	
    if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"autoindex" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && file[i] != ';' && file[i] != '#') {
			if (arg > 0)
				throw std::invalid_argument("invalid number of arguments in \"autoindex\" directive");
			arg++;
			int skip = skip_word(file, i);
			std::string word = file.substr(i, skip - i);
			if (word == "on")
				autoindex = true;
			else if (word == "off")
				autoindex = false;
			else
				throw std::invalid_argument("invalid value " + word + " in \"autoindex\" directive, it must be \"on\" or \"off\"");
			i = skip;
		}
	}
	a.set_autoindex(autoindex);
	if (file[i] == ';')
		++i;
	return i;
}