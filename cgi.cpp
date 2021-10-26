#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <fstream>

// sudo apt install libsqlite3-dev libxml2-dev
// ./configure --without-iconv

// REQUEST_METHOD=GET
// SERVER_PROTOCOL=HTTP/1.1
// PATH_INFO=/

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	int pid;

	int fd = open("php", O_RDONLY);
	char *tab[] { "php-cgi", 0};
	pid = fork();
	if (pid == 0) {
		dup2(fd, 0);
		execve("/usr/bin/php-cgi", tab, envp);
		std::cout << "FUCK" << std::endl;
		return 0;
	}
	close(fd);
	return 0;
}