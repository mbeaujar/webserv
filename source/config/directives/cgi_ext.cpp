#include "../../prototype.hpp"
#include "../server/Location.hpp"

int parse_cgi_ext(std::string file, int i, Location &a)
{
	int count = 0;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"fastcgi_extension" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#') {
			
			int skip = skip_word(file, i);
			std::string word = file.substr(i, skip - i);
			if (word[0] != '.')
				throw std::invalid_argument("invalid argument in \"fastcgi_extension\" directive");
			a.set_cgi_ext(word);
			i = skip;
			count++;
		}
	}
	if (count != 1)
		throw std::invalid_argument("invalid number of arguments in \"fastcgi_extension\" directive");
	if (file[i] == ';')
		++i;
	return i;
}