#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int f1 = open(argv[1], O_RDONLY);
	int f2 = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);

	if(f1 == -1 || f2 == -1)
		exit(2);

	char buffer[1024];
	int curr = 1;

	int chars_written = 0;

	while((curr = read(f1, buffer, sizeof(buffer))) > 0){
		write(1, buffer, curr);
	
		for(int i = 0; i < curr; i++){
			if(buffer[i] == '\n'){
				chars_written = 0;
			} else if (chars_written < 3){
				write(f2, &buffer[i], 1);
				chars_written++;
			}
		}	
	}	

	close(f1);
	close(f2);

	return 0;
}
