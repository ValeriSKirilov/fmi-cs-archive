#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int f1 = open("task2helper.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(f1 == -1)
		exit(2);
	
	write(f1, "abcdefghijklmnopqrstuvwxyz\n", 26);
	lseek(f1, 0, SEEK_SET);

	char buffer[10];
	int size = 10;
	int curr = 1;
	while((curr = read(f1, buffer, 10)) > 0){
		write(1, buffer, curr);
		printf("\n");
	}	

	return 0;
}
