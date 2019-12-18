#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#ifndef SIZE
#define SIZE 8
#endif

int main(int argc, char const *argv[])
{
	int fd, ret;
	char buff[SIZE];

	fd = open("/dev/Cirbuff", O_RDWR);
	if (fd < 0)
	{
		perror("Device not found\n");
		return fd;
	}

	ret = read(fd, buff, sizeof(buff));
	printf("Read from kernel: %s | Count:%d\n", buff, ret);
	return 0;
}