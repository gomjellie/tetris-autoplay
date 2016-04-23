#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void){
	char arr[8][4]={
		"▤",
		"▢",
	};
	while(1){

	printf("%s", arr[0]);

	printf("size: %ld", sizeof("▢"));
	printf("long int: %ld", sizeof(long int));

//	printf("%c\n", arr[0]);
//	printf("%c\n", arr[1]);
	}
	
	return 0;
}
