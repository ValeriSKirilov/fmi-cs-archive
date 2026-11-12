#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc < 2)
		exit(2);

	int f = open(argv[1], O_RDONLY);

	if(f == -1)
		exit(2);

	int m, n;

	if(scanf("%d-%d", &m, &n) != 2)
		return 1;

	char buffer[1024];
	int curr = 1;
	int current_col = 1;

	while((curr = read(f, buffer, sizeof(buffer))) > 0){
		for(int i = 0; i < curr; i++){
			if(buffer[i] == '\n'){
				write(1, &buffer[i], 1);
				current_col = 1;
			}
			else{
				if(m <= current_col && current_col <= n){
					write(1, &buffer[i], 1);
				}
			current_col++;
			}
		}
	}

	return 0;
}
