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
	return i;
}

int skip_comment(std::string file, int i) {
	if (file[i] == '#')
		i++;
	else
		return i;
	while (file[i] && file[i] != '\n')
		i++;
	if (file[i] == '\n')
		i++;
	return i;
}

int skip_word(std::string file, int i) {
	while (file[i] && !isspace(file[i]) && file[i] != ';' && file[i] != '#')
		i++;
	return i;
}