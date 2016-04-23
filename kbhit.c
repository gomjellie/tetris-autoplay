#include <stdio.h>
#include <termios.h>

int main(void){
	char a='\0';
	int i=0;

	while(1){

		if(kbhit()){
			scanf("%c", &a);
		}
		printf("asdf");
	}
	return 0;
}
