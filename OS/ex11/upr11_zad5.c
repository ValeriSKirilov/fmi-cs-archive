#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc < 2)
		exit(2);
	
	int f = open(argv[1], O_RDONLY);

	if(f == -1)
		exit(2);

	int wCount = 0, lCount = 0, bCount = 0;

	char buffer[1024];
	int curr = 1;

	int in_word = 0;

	while((curr = read(f, buffer, sizeof(buffer))) > 0){
		for(int i = 0; i < curr; i++){
			if(buffer[i] == '\n'){
				lCount++;
				in_word = 0;
			}
			else if(buffer[i] == ' ' || buffer[i] == '\t'){
				in_word = 0;
			}
			else{
				if(in_word == 0)
				{
					wCount++;
					in_word = 1;
				}
			}
		}
		bCount += curr;
	}

	printf("%d %d %d %s\n", lCount, wCount, bCount, argv[1]);

	close(f);

	return 0;
}
