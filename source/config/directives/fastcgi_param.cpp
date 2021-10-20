#include "../../prototype.hpp"
#include "../server/Location.hpp"
#include <string>

int parse_fastcgi_param(std::string file, int i, Location &a)
{
    int count = 0;

	if (!isspace(file[i]))
		throw std::invalid_argument("unknow directive \"index" + file.substr(i, skip_word_exception(file, i) - i) + "\"");
	while (file[i] && file[i] != ';')
	{
		i = skip_space(file, i);
		i = skip_comment(file, i);
		if (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#') {
			
			int skip = skip_word(file, i);
			std::string word = file.substr(i, skip - i);
			a.adding_param(word);
			i = skip;
			count++;
		}
	}
	if (count == 0)
		throw std::invalid_argument("invalid number of arguments in \"fastcgi_param\" directive");
	if (file[i] == ';')
		i++;
	return i;
}

int parse_key(std::string str)
{
	if (str.compare("SERVER_SOFTWARE") == 0) 
		return (101);
	else if (str.compare("QUERY_STRING") == 0) 
		return (102);
	else if (str.compare("REQUEST_METHOD") == 0) 
		return (103);
	else if (str.compare("CONTENT_TYPE") == 0) 
		return (104);
	else if (str.compare("CONTENT_LENGTH") == 0) 
		return (105);
	else if (str.compare("SCRIPT_FILENAME") == 0) 
		return (106);
	else if (str.compare("SCRIPT_NAME") == 0) 
		return (107);
	else if (str.compare("REQUEST_URI") == 0) 
		return (108);
	else if (str.compare("DOCUMENT_URI") == 0) 
		return (109);
	else if (str.compare("DOCUMENT_ROOT") == 0) 
		return (110);
	else if (str.compare("SERVER_PROTOCOL") == 0) 
		return (111);
	else if (str.compare("REMOTE_ADDR") == 0) 
		return (112);
	else if (str.compare("REMOTE_PORT") == 0) 
		return (113);
	else if (str.compare("SERVER_ADDR") == 0) 
		return (114);
	else if (str.compare("SERVER_PORT") == 0) 
		return (115);
	else if (str.compare("SERVER_NAME") == 0) 
		return (116);
	throw std::invalid_argument("\"fastcgi_param\" invalid key name");
	return (0);
}