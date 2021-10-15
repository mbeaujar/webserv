#include <iostream>
#include "../../prototype.hpp"

int skip_space(std::string file, int i) {
	while (file[i] && isspace(file[i]))
		i++;
	return i;
}

int skip_bracket(std::string file, int i) {
	int counter = 0;
	while (file[i]) {
		i = skip_comment(file, i);
		if (file[i] == '{')
			counter++;
		if (file[i] == '}')
			counter--;
		if (counter == 0)
			break;
		i++;
	}
	if (!file[i])
		throw std::invalid_argument("unknow directive");
	return i;
}

int skip_comment(std::string file, int i) {
	while (file[i] && file[i] == '#') {
		while (file[i] && file[i] != '\n')
			i++;
		if (file[i] == '\n')
			i++;
	}
	return i;
}

int skip_word_exception(std::string file, int i) {
	while (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#')
		i++;
	return i;
}

int skip_word(std::string file, int i) {
	while (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#') {
		if (file[i] == '{' || file[i] == '}')
			throw std::invalid_argument("directive is not termined by \";\"");
		i++;
	}
	return i;
}