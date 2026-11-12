#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int f = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	
	if(f == -1)
		exit(2);

	int n;
	char buffer[4096];

	while((n = read(0, buffer, 4096)) > 0){
		write(f, buffer, n);
	}

	return 0;	
}
