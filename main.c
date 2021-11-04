#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd = open("error.log", O_RDWR);
	if (fd == -1)
		printf("fd can't open\n");
	fd_set test;
	FD_ZERO(&test);
	FD_SET(200, &test);
	FD_SET(fd, &test);
	printf("%d\n", select(3, &test, NULL, NULL, NULL));
	printf("%s\n", strerror(errno));
	printf("200: %d\tfd: %d\n", FD_ISSET(200, &test), FD_ISSET(fd, &test));
	return 0;
}