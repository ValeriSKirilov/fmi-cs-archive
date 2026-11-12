#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int f = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(f == -1)
		exit(1);
	
	char buffer[1024];
	int curr = 1;
	
	while((curr = read(0, buffer, sizeof(buffer))) > 0){
			write(f, buffer, curr);
			for(int i = 0; i < curr; i++){
				if(buffer[i] == '\t')
					write(2, ">>>", 3);
				else
					write(2, &buffer[i], 1);
			}
	}
	close(f);
	
	return 0;
}
