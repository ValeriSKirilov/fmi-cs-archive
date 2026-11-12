#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int status1, status2;

	if(fork()){
		wait(&status1);
		if(fork()){
			wait(&status2);
			printf("%d", status2);
		}
		else{
			if(execvp(argv[2], &argv[2]) == -1)
				printf("Error executing %s", argv[2]);
		}
	}
	else{
		if(execlp("grep", "grep", "int", argv[1], NULL) == -1)
			printf("Error executing grep");
	}
	
	return 0;
}
