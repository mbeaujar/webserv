#include "prototype.hpp"

int parse_client_size(std::string file, int i, Location &a)
{
    int max_size = -1;

    if (!isspace(file[i]))
        throw std::invalid_argument("unknow directive \"max_body" + file.substr(i, skip_word_exception(file, i) - i) +
                                    "\"");
    while (file[i] && file[i] != ';')
    {
        i = skip_space(file, i);
        i = skip_comment(file, i);
        if (file[i] && !isspace(file[i]) && file[i] != '#' && file[i] != ';')
        {
            if (max_size == -1 && isdigit(file[i]))
            {
                max_size = recup_nb(file, i);
                if (file[i] == 'm' || file[i] == 'M')
                    ++i;
            }
            else
                throw std::invalid_argument("\"max_body\" directive invalid value");
        }
    }
    if (!file[i])
        throw std::invalid_argument("unexpected \"}\"");
    if (max_size == -1)
        throw std::invalid_argument("invalid number of arguments in \"max_body\"");
    a.set_max_body(max_size);
    if (file[i] == ';')
        ++i;
    return i;
}