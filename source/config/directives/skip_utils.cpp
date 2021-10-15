#include <iostream>
#include "../../prototype.hpp"


int recup_nb(std::string file, int & i) {
	int port = 0;

	while (isdigit(file[i])) {
		port = (port * 10) +  (file[i] - 48);
		i++;
	}
	return port;
}

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
		throw std::invalid_argument("unexpected \"}\" or \"{\"");
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