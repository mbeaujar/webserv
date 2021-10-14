#include "../../prototype.hpp"
#include "../server/Location.hpp"

int	parse_autoindex(std::string file, int i, Location &a)
{
    int arg = 0;
    bool autoindex = false;
    std::string state;
	
    if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"autoindex" + file.substr(i, skip_word(file, i) - i) + "\"");
    while (file[i] && file[i] != ';')
	{
        int tmp = i;
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && file[i] != ';' && file[i] != '#') {
            if (arg > 0)
                throw std::invalid_argument("invalid number of arguments in \"autoindex\" directive");
            arg++;
            if (file.substr(i, skip_word(file, i) - i) == "on")
                autoindex = true;
            else if (file.substr(i, skip_word(file, i) - i) == "off")
                autoindex = false;
            else
                throw std::invalid_argument("invalid value " + file.substr(tmp, skip_word(file, i) - i) + " in \"autoindex\" directive, it must be \"on\" or \"off\""); 
			i = skip_word(file, i);
		}
	}
	a.set_autoindex(autoindex);
	return i;
}