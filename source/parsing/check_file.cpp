#include <string>
#include <iostream>

int skip_bracket(std::string file, int begin) {
	int counter = 0, i = begin;
	for (; file[i]; i++) {
		if (file[i] == '{')
			counter++;
		if (file[i] == '}')
			counter--;
		if (counter == 0)
			return i;
	}
	return i;
}

int check_bracket(std::string file) {
    int a = 0, b = 0;
    for (int i = 0; file[i]; i++) {
        int counter = 0;
        if (file[i] == '{')
            a++, counter++;
        else if (file[i] == '}')
            b++, counter--;
        if (counter < 0)
            return 1;
    }
    if (a - b != 0)
        return 1;
    return 0;
}

int clean_file(std::string file) {
    if (check_bracket(file))
        return 1;
    for (int i = 0; file[i]; i++) {
        int counter = 0;
        if (file[i] == '{')
            counter++;
        else if (file[i] == '}')
            counter--;
		// i = skip_bracket(file, i);
        if (counter == 0) {
            while (file[i]) {
                if (file[i] == '#')
                    while (file[i] && file[i] != '\n')
                        i++;
                if (isspace(file[i]))
                    i++;
                else if (!file.compare(i, 6, "server"))
                    i += 6;
                else if (file[i] == '{')
                    break;
                else
                    return 1;
			}
        }
    }
    return 0;
}