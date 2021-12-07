#include "prototype.hpp"

int parse_cgi(std::string file, int i, Location & a)
{
	int count = 0;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"cgi " + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#') {
			
			int skip = skip_word(file, i);
			std::string ext = file.substr(i, skip - i);
			if (ext[0] != '.')
				throw std::invalid_argument("invalid argument in \"cgi\" directive");
			i = skip_space(file, skip);
            skip = skip_word(file, i);
			std::string path = file.substr(i, skip - i);
			if (file_exist(path) == false)
				throw std::invalid_argument("invalid argument in \"cgi\" directive, path does not exist");
            a.set_cgi(ext, path);
			i = skip;
			count++;
		}
	}
	if (count != 1)
		throw std::invalid_argument("invalid number of arguments in \"cgi\" directive");
	if (file[i] == ';')
		++i;
	return i;
}