#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int n;
	char buffer[4096];

	for(int i = 0; i < argc; i++){
		int f = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}

	while((n = read(0, buffer, 4096)) > 0){
		for(int i = 0; i < argc; i++){
			int f = open(argv[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
			write(f, buffer, n);
		}
		write(1, buffer, n);
	}

	return 0;
}
