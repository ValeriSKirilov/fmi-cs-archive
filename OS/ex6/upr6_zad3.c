#include <stdio.h>

int main(){
	int arr[10];
	int neg_cnt = 0;

	for(int i = 0; i < 10; i++){
		printf("Number %d: ", i + 1);
		scanf("%d", &arr[i]);	
	}
	
	printf("\nNegative numbers: ");
	
	for(int i = 0; i < 10; i++){
		if(arr[i] < 0){
			printf("%d ", arr[i]);
			neg_cnt++;
		}	
	}

	printf("\nNeg num count: %d\n", neg_cnt);

	return 0;
}
