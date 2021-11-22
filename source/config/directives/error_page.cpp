#include <iostream>
#include "../server/Server.hpp"
#include "../../prototype.hpp"

void braket_check(std::string file, int i)
{
    if (file[i] == '{')
        throw std::invalid_argument("directive \"error_page\" is not terminated by \";\" in");
    if (file[i] == '}')
        throw std::invalid_argument("unexpected \"}\" in ...");
}

int	parse_error_page(std::string file, int i, Server &a)
{
    int code = 0;
    int error = 0;
    int semicolon = 0;
    std::string value;

    if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"error_page" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';' && file[i] != '{')
	{
        if (file[i] == ';')
            semicolon = 1;
		i = skip_space(file, i);
		i = skip_comment(file, i);
        if (file[i] && file[i] != ';' && file[i] != '#') {
            if (code == 0)
            {
                int tmp = i;
                code = 1;
                while (isdigit(file[i])) {
                    error = (error * 10) + (file[i] - 48);
                    i++;
                }
                if (!isspace(file[i]))
                    throw std::invalid_argument("invalid value \"" + file.substr(tmp, skip_word_exception(file, i) - tmp) + "\"" + " in ...");
                braket_check(file, i);
                if (error < 300 || error > 599)
                    throw std::invalid_argument("value \"" + file.substr(tmp, skip_word_exception(file, i) - tmp) + "\" must be between 300 and 599 in ...");
            }
            else if (code == 1)
            {
                braket_check(file, i);
                code++;
			    value = file.substr(i, skip_word(file, i) - i);
			    i = skip_word(file, i);
            }
            else {
                braket_check(file, i);
                throw std::invalid_argument("invalid value \"" + value + "\"" + " in ...");
            }
		}
	}
    if (file[i] == '{')
        throw std::invalid_argument("directive \"error_page\" is not terminated by \";\" in ...");
    if (file[i] != ';' || code != 2)
        throw std::invalid_argument("invalid number of arguments in \"error_page\" directive in ...");
	if (file_exist(value) == false)
		throw std::invalid_argument("invalid argument in \"error_page\" directive, path does not exist");
	a.adding_error_page(error, value);
    if (file[i] && file[i] == ';')
        i++;
    return i;
}
