#include "../../prototype.hpp"
#include "../server/Location.hpp"

int	parse_autoindex(std::string file, int i, Location &a)
{
    int arg = 0;
    bool autoindex;
    std::string state;
	
    if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"autoindex" + file.substr(i, skip_word(file, i) - i) + "\"");
    while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && file[i] != ';' && file[i] != '#') {
            if (argv)
                throw std::invalid_argument("invalid number of arguments in \"autoindex\" directive");
            arg++;
            if (file.substr(i, skip_word(file, i) - i) == "on")
                autoindex = true;
            else if (file.substr(i, skip_word(file, i) - i) == "off")
                autoindex = false;
            else
                throw std::invalid_argument("wtf"); 
			i = skip_word(file, i);
		}
	}
	return i;
}