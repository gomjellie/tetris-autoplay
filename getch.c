#include <stdio.h>
#include <curses.h>
#include <termios.h>

int main(void){
	char i=0,j=0;
	
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	cbreak();
	for(j=0;j<10;j++){
		printf("insert:");
		i=getch();
		if(i!='j')
			printf("err");
		move(1,j);
		printf("%d resulet: %c \n",j,i);
	}
	
	endwin();
}
