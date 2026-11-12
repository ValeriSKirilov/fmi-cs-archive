#include <stdio.h>

int main(){
	char buffer[256];

	printf("Enter text: ");

	fgets(buffer, sizeof(buffer), stdin);
	printf("You entered: %s", buffer);	

	return 0;
}
