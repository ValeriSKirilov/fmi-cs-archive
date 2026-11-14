#include <stdio.h>
#include <unistd.h>

int main(){
	int pid;
	
	pid = fork();
	if(pid == 0){
	}
	else{
		int i = 0;
		while(i < 10000000)
			i++;
	}
	printf("%d\n", getpid());

	return 0;
}
