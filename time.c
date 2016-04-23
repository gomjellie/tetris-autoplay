#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int TIME=0;
int main(void){
	int i=0;
	time_t TIME;
	for(time(&TIME);TIME%3!=0;time(&TIME)){
		scanf("%d", &i);
		printf("%d", i);

	}
	printf("\n");

}
