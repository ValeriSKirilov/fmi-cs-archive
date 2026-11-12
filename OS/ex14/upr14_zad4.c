#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int fd = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, 0644);

	if(fd == -1)
		exit(2);

	int commandSize = 0, i = 0;
	while(argv[1][i] != '\0'){
		commandSize++;
		i++;
	}

	int status;
	if(fork()){
		wait(&status);
		if(status == 0)
			write(fd, argv[1], commandSize);
		else
			printf("The first command's execution failed");	
	}
	else{
		if(execlp(argv[1], argv[1], NULL)){
			printf("Error executing %s", argv[1]);
			exit(2);
		}
	}

	if(execlp(argv[2], argv[2], NULL) == -1){
		printf("Error executing %s", argv[2]);
		exit(2);
	}

	return 0;
}
