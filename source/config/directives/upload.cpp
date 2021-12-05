#include "../../prototype.hpp"
#include "../server/Location.hpp"

int parse_upload(std::string file, int i, Location &a)
{
	int count = 0;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"upload" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#') {
			
			int skip = skip_word(file, i);
			std::string word = file.substr(i, skip - i);
			if (word.length() > 1 && word[word.length()-1] == '/')
				word.erase(--word.end());
			if (file_exist(word) == false || is_directory(word) == false)
				throw std::invalid_argument("invalid argument in \"upload\" directive, path does not exist");
			a.set_upload(word);
			i = skip;
			count++;
		}
	}
	if (count != 1)
		throw std::invalid_argument("invalid number of arguments in \"upload\" directive");
	if (file[i] == ';')
		++i;
	return i;
}