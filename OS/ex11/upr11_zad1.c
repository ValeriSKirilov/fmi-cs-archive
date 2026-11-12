#include <fcntl.h>

int main(int argc, char *argv[]){
	int fd1 = open(argv[1], O_RDONLY);
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd1 == -1)
	{
       		exit(2);
	}

	int buffer[1024];
	int size = 4096;
	int curr = 1;
	while ((curr = read(fd1, buffer, size)) > 0)
	{
		write(fd2, buffer, curr);	
	}

	return 0;
}
