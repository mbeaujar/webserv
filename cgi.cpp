#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fstream>

// sudo apt install libsqlite3-dev libxml2-dev
// ./configure --without-iconv

std::string read_file(std::string filename) 
{
    std::string	    content, line;
    std::ifstream	file;

    file.open(filename.c_str());
    while (std::getline(file, line))
        content += line + "\n";
	file.close();
    return content;
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	int pid;
	std::string content = read_file("");
	pid = fork();
	if (pid == 0) {

	}
	return 0;
}