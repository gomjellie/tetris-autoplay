#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>//curses.h replaces <conio.h> you have to add -lcurses at the end
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define LINE 2
#define BLOCK 1
#define EMPTY 0

#define X_LEFT_BOARDER 5
#define X_RIGHT_BOARDER 17
#define X_LEN 12
#define Y_TOP_BOARDER 3
#define Y_BOT_BOARDER 28
#define Y_LEN 25

const int SPEED=1300;
int INDEX=0;
int SCORE=0;
char COLOUR[7][4]={
	"▢",
	"♡",
	"ペ",
	"♮",
	"★",
	"の",
	"♯"
};
int BOARD_ARR[Y_LEN][X_LEN]={EMPTY,};
const int _BLOCK_ARR[7][4][4]={
	{
		{0,0,0,0},
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{2,2,2,0},
		{0,2,0,0},
		{0,0,0,0}
	},
	{
		{0,0,3,0},
		{0,0,3,0},
		{0,3,3,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{0,4,4,0},
		{0,0,4,0},
		{0,0,4,0}
	},
	{
		{0,0,0,0},
		{0,5,5,0},
		{5,5,0,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{6,6,0,0},
		{0,6,6,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{0,7,7,0},
		{0,7,7,0},
		{0,0,0,0}
	}
};
int BLOCK_ARR[4][4]={
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{1,1,1,1}
}
;
int BIT_X=4,BIT_Y=1;
int BIT_Z=0
;
enum {LEFT_KEY=1,DOWN_KEY,RIGHT_KEY,UP_KEY,QUIT,SPACE_KEY};

void gotoXY(int x,int y);
int writeBlockOnBoard(void);
char getKey(void);
void eraseBlockOffBoard(void);
void gotoXY(int x,int y);
void initGame(void);
void drawBoarder(void);
void drawInsideGame(void);
char getNextMove(void);
void rotateBlock(void);
int collision(void);
void stack(void);
void moveBit(char ch);
void gravity(int y);
int clearLine(void);
int kbhit(void);
void indexCheck(void);
void randomNextBlock(void);

int main(void)
{
	int x=0,y=0,i=0;
	char NXT='\0';
	initGame();
	drawBoarder();
	randomNextBlock();

	while(NXT!=QUIT){
		eraseBlockOffBoard();
		moveBit(NXT);
		writeBlockOnBoard();
		drawInsideGame();
		NXT=getNextMove();
		indexCheck();
	}
	endwin();
	return 0;
}

int writeBlockOnBoard(void){
	int x=0,y=0;
	for(x=0;x<4;x++){
		for(y=0;y<4;y++){
			if(BLOCK_ARR[y][x]!=EMPTY){
				BOARD_ARR[BIT_Y+y][BIT_X+x]=BIT_Z+1;
			}
		}
	}
	return 0;
}

char getKey(void){
	char key[3]={'\0',};
	key[0]=getchar();
	return key[0];
}

void eraseBlockOffBoard(void){
	int x=0,y=0;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			if(BLOCK_ARR[y][x]!=EMPTY){
				BOARD_ARR[y+BIT_Y][x+BIT_X]=EMPTY;
			}
		}
	}
}

void gotoXY(int x,int y){
	printf("\033[%dd\033[%dG",y,x);
}

/*void ClearAll(void){
	printf("\033[2J");
}*/  //replaced with curses.h clear()
void initGame(void){
	initscr();//curses.h
	cbreak();//curses.h
	keypad(stdscr, TRUE);//curses.h
	noecho();//curses.h
	clear();
	srand((unsigned int)time(NULL));
	int x=0,y=0;
	for(x=0;x<X_RIGHT_BOARDER-X_LEFT_BOARDER;x++){
		BOARD_ARR[0][x]=LINE;
		BOARD_ARR[Y_LEN-1][x]=LINE;
	}
	for(y=0;y<Y_BOT_BOARDER-Y_TOP_BOARDER;y++){
		BOARD_ARR[y][0]=LINE;
		BOARD_ARR[y][X_LEN-1]=LINE;
	}
	gotoXY(40,INDEX++);printf("debug");
}

void drawBoarder(void){
	int x=0,y=0,k=0;
	for(x=0,k=0;x<X_LEN;x++,k++){
	refresh();
		for(y=0;y<Y_LEN;y++){
			if(BOARD_ARR[y][x]==LINE){
				gotoXY(k+x+X_LEFT_BOARDER,y+Y_TOP_BOARDER);printf("▤");
			}
		}
	}
	gotoXY(6,2);printf("gomjellie's TETRIS GAME");
}

char getNextMove(void){
	char key='\0';
	char out='\0';
	int i=0;

	if(kbhit()){
		key=getKey();
	}
	else if(!kbhit()){
		out=DOWN_KEY;
		for(i=0;i<SPEED;i++){
			if(kbhit()){
				//key=getKey();//넣으면 벽에서 방향키 계속누를때 벽에 붙어서 안내려감
				//근데 안넣으면 벽에 흘러내려감
				break;
			}
			usleep(100);
		}
	}
	switch(key){
		case 'H':
		case 'h':
			out=LEFT_KEY;break;
		case 'J':
		case 'j':
			out=DOWN_KEY;break;
		case 'L':
		case 'l':
			out=RIGHT_KEY;break;
		case 'K':
		case 'k':
			out=UP_KEY;break; 
		case ' ':
			out=SPACE_KEY;break;
		case 'Q':
		case 'q':
			out=QUIT;
	}
	return out;
}

void rotateBlock(void){
	char tmpArr[4][4];
	int x=0,y=0;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			tmpArr[y][x]=BLOCK_ARR[y][x];
		}
	}
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			BLOCK_ARR[x][3-y]=tmpArr[y][x];
		}
	}
}

int collision(void){
	int x=0,y=0;
	int collision=0;
	for(x=0;x<4;x++){
		for(y=0;y<4;y++){
			if(BLOCK_ARR[y][x]!=EMPTY){
				if(BOARD_ARR[BIT_Y+y][BIT_X+x]!=EMPTY){
					gotoXY(40,INDEX++);printf("collision");
					return 1;
				}
			}
		}
	}
	return 0;
}//works well

void stack(void){
	gotoXY(40,INDEX++);printf("stack           ");
	BIT_Y--;
	writeBlockOnBoard();
	randomNextBlock();
	BIT_Y=0;BIT_X=4;//BIT_Z=random()%7;
	clearLine();
}

void moveBit(char ch){
	int prevZ=0;
	switch(ch){
		case DOWN_KEY:
			BIT_Y++;
			if(collision()){
				stack();
			}
			break;
		case UP_KEY:
			rotateBlock();
			if(collision()){
				rotateBlock();rotateBlock();rotateBlock();
			}
			break;
		case LEFT_KEY:
			BIT_X--;
			if(collision()) BIT_X++;break;
		case RIGHT_KEY:
			BIT_X++;
			if(collision())
				BIT_X--;
			break;
		case SPACE_KEY:
			while(!collision()){
				BIT_Y++;
			}
			BIT_Y--;
			//stack();
			break;
		default:
			break;
	}
	gotoXY(40,INDEX++);
	printf("CURS: %d %02d",BIT_X,BIT_Y);
	gotoXY(40,INDEX++);
	printf("SCORE: %d", SCORE);
}

void gravity(int y) {
	int x = 1;
	for (;y >1;y--) {
		for (x = 1; x < X_LEN - 1; x++) {
			BOARD_ARR[y][x] = BOARD_ARR[y-1][x];
		}
	}
}

int clearLine(void){
	int x=0,y=0,cnt;
	for(y=Y_LEN-2;y>1;y--){
		cnt=0;
		for(x=1;x<X_LEN-1;x++){
			if(BOARD_ARR[y][x]!=EMPTY)
				cnt+=1;
		}
		if(cnt==1*(X_LEN-2)){
			SCORE+=10;
			gravity(y);
			y=Y_LEN-1;
		}
	}
}

int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

void drawInsideGame(void){
	int x=0,y=0,k=0;
	for(y=1;y<Y_LEN-1;y++){
		for(x=1,k=1;x<X_LEN-1;x++,k++){
			if(BOARD_ARR[y][x]==EMPTY){
				gotoXY(k+x+X_LEFT_BOARDER,y+Y_TOP_BOARDER);
				printf(" ");
			}
			else if(BOARD_ARR[y][x]>=1&&BOARD_ARR[y][x]<8){
				gotoXY(k+x+X_LEFT_BOARDER,y+Y_TOP_BOARDER);
				printf("%s", COLOUR[BOARD_ARR[y][x]-1]);
			}
		}
		refresh();
	}
}

void indexCheck(void){
	int i=0;
	if(INDEX>30){
		for(i=0;i<=33;i++){
			gotoXY(40,i);
			printf("            ");
		}
		INDEX=0;
	}
}

void randomNextBlock(void){
	int x,y;
	BIT_Z=rand()%7;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			BLOCK_ARR[y][x]=_BLOCK_ARR[BIT_Z][y][x];
		}
	}
}
