#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>//curses.h replaces <conio.h> you have to add -lcurses at the end
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#define LINE 2
#define BLOCK 1
#define EMPTY 0

#define X_LEFT_BOARDER 5
#define X_RIGHT_BOARDER 17 //17
#define X_LEN 12  //12
#define Y_TOP_BOARDER 3
#define Y_BOT_BOARDER 33 //28
#define Y_LEN 30 //25

const int SPEED=1000;
const int AISPEED=0;//25000 , 2000
int INDEX=0;
int AIINDEX=0;
int SCORE=0;
char COLOUR[7][4]={
	"M",
	"#",
	"$",
	"%",
	"&",
	"K",
	"P"
};
int BOARD_ARR[Y_LEN][X_LEN]={EMPTY,};
const int _BLOCK_ARR[7][4][4]={
	{
		{0,1,0,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,1,0,0}
	},
	{
		{0,0,0,0},
		{0,0,2,0},
		{0,2,2,0},
		{0,0,2,0}
	},
	{
		{0,0,0,0},
		{0,3,0,0},
		{0,3,3,3},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{0,0,4,0},
		{4,4,4,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{0,5,5,0},
		{5,5,0,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{0,6,6,0},
		{0,0,6,6},
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
	{0,0,0,0}
};

int BIT_X=4,BIT_Y=1;
int BIT_Z=0;

typedef struct position{
	int x,y;
	int rotation;
	int score;
}Pos;

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
int getIdealPoint(void);
Pos findIdealPosition(void);
int getClearLinePoint(void);

void debugBestPosition(Pos ptr){
	gotoXY(60,AIINDEX++);
	printf("%d %d %d %d", ptr.x, ptr.y, ptr.rotation, ptr.score);
};

void animationeffect(double par){
	writeBlockOnBoard();
	drawInsideGame();
	eraseBlockOffBoard();
	//usleep(AISPEED*par);
}

void moveBestPos(Pos bestPosition, char *nxt){
	int i;
	BIT_Y=1;BIT_X=5;
	for(i=0;i<bestPosition.rotation;i++){
		rotateBlock();
		if(collision())
			*nxt=QUIT;
		animationeffect(7);
	}
	while(BIT_X!=bestPosition.x){
		if(BIT_X>bestPosition.x){
			BIT_X--;	
			if(collision())
				*nxt=QUIT;animationeffect(4);
		}
		else if(BIT_X<bestPosition.x){
			BIT_X++;
			if(collision())
				*nxt=QUIT;animationeffect(4);
		}
//			bestPosition.x=BIT_X;
	}
	animationeffect(4);
		while(!collision()){
		animationeffect(0.5);
		BIT_Y++;
	}
	stack();
	return ;
}

int main(void)
{
	int x=0,y=0,i=0;
	char NXT='\0';
	Pos bestPosition;
	initGame();
	drawBoarder();
	randomNextBlock();

	while(NXT!=QUIT){
		bestPosition=findIdealPosition();
		moveBestPos(bestPosition, &NXT);
//		moveBit(NXT);
		writeBlockOnBoard();
		drawInsideGame();
		eraseBlockOffBoard();
		
//		NXT=getNextMove();
		//debugBestPosition(bestPosition);
		
		indexCheck();
	}
	//system("echo %d > score.txt", SCORE);
	endwin();
	printf("You Scored %d!\n", SCORE);
//	system("./tetris");//끝나고 재시작옵션
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
	curs_set(0);// hide curs on linux option
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
	gotoXY(4,2);printf("https://github.com/gomjellie");
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
			//usleep(100);
		}
	}
	switch(key){
/*		case 'H':
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
			out=SPACE_KEY;break;*/
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
	//				gotoXY(40,INDEX++);printf("collision");
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
	BIT_Y=1;BIT_X=4;//BIT_Z=random()%7;
	clearLine();
}

void moveBit(char ch){
//	int prevZ=0;
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
			if(collision()) BIT_X++;
			break;
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
//	gotoXY(40,INDEX++);
//	printf("CURS: %d %02d",BIT_X,BIT_Y);
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
			gotoXY(40,INDEX++);
			printf("SCORE: %d", SCORE);


			gravity(y);
			y=Y_LEN-1;
		}
	}
		return 0;
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
			else if(BOARD_ARR[y][x]!=0){
//			else if(BOARD_ARR[y][x]>=1&&BOARD_ARR[y][x]<8){
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
	if(AIINDEX>30){
		for(i=0;i<=34;i++){
			gotoXY(60,i);
			printf("           ");
		}
		AIINDEX=0;
	}
}

void randomNextBlock(void){
	int x,y;
	//srand((unsigned int)time(NULL));//
	int prevBIT_Z=BIT_Z;
	while(prevBIT_Z==(BIT_Z=rand()%7));
//	BIT_Z=rand()%7;
//	BIT_Z=3;//특정블록만 나오게 함(테스트용)
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			BLOCK_ARR[y][x]=_BLOCK_ARR[BIT_Z][y][x];
		}
	}
}

int getIdealPoint(void){
	int boardIdealPoint=0;
	int x=0,y=0;
	for(y=2;y<Y_LEN-1;y++){
		for(x=1;x<X_LEN-1;x++){
			if(BOARD_ARR[y][x]!=EMPTY){
				boardIdealPoint+=y;
			}
		}
	}
	/*
	for(y=BIT_Y;y<BIT_Y+4;y++){
		for(x=BIT_X-1;x<BIT_X+5;x++){
			if(BOARD_ARR[y][x]!=EMPTY)
				boardIdealPoint+=1;
		}
	}
	*/
	//새로 만들고 있는부분
	
	return boardIdealPoint;
}
int getAdjacentPoint(void){
	int adjacentPoint=0;
	int x=0,y=0;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			if(BLOCK_ARR[y][x]!=EMPTY){
				if(BOARD_ARR[BIT_Y+y][BIT_X+x-1]!=EMPTY){
					adjacentPoint+=BIT_Y+y;
				}
				if(BOARD_ARR[BIT_Y+y][BIT_X+x+1]!=EMPTY)
					adjacentPoint+=BIT_Y+y;
			}
		}
	}
	return adjacentPoint;
}

int getMinusPoint(void){
	int boardMinusPoint=0;
	int x=0,y=0;
	int height=0;
	
	for(x=1;x<X_LEN-1;x++){
		for(y=2;BOARD_ARR[y][x]==EMPTY;y++){
			height=y;
		}
		for(y=Y_LEN-1;y>height;y--){
			if(BOARD_ARR[y][x]==EMPTY){
//				boardMinusPoint+=(10+Y_LEN-y);
				boardMinusPoint+=y;
			}
		}
	}
	return boardMinusPoint;
}
int getClearLinePoint(void){
	int clearLinePoint=0;
	int x=0,y=0;
	int count=0;//lineCount=0;
	for(y=BIT_Y;y<BIT_Y+4;y++){
		for(x=1,count=0;x<X_LEN-1;x++){
			if(BOARD_ARR[y][x]!=EMPTY){
				count++;
			}
		}
/*		
		if(count==X_LEN-2){
			clearLinePoint+=(Y_LEN-y)*(X_LEN-2);
		}
		if(count==X_LEN-3){
			clearLinePoint+=(Y_LEN-y-3)*(X_LEN-3);
		}
*/

		if(count==X_LEN-2){
			clearLinePoint+=y*(X_LEN-2);
//			lineCount++;
		}
		if(count==X_LEN-3){
//			clearLinePoint+=(y-3)*(X_LEN-3);
//			lineCount++;
		}
	}
//	if(lineCount==1&&BIT_Z==3)
//		return clearLinePoint/2;
	return clearLinePoint;
}

Pos findIdealPosition(void){
	int firstX=BIT_X,firstY=BIT_Y;
	Pos position[4]={0,};
	int highestScore=0, plusScore=0, minusScore=0, totalScore=0;
	int rotation=0,bestRotation=0;
	int i=0;

	for(rotation=0;rotation<4;rotation++){
		for(BIT_X=0,i=0;1;BIT_X++){
			BIT_Y=1;

			if(i==0&&collision()){
				BIT_X=1;i++;
			}
			else if(collision()){
				break;
			}
			while(!collision()){
				BIT_Y++;
			}
			BIT_Y--;
			writeBlockOnBoard();
			plusScore=4*getIdealPoint()+5*getClearLinePoint()+5*getAdjacentPoint();
			minusScore=16*getMinusPoint();
			totalScore=plusScore-minusScore;
			eraseBlockOffBoard();

			if(highestScore<=totalScore){
				highestScore=totalScore;
				position[rotation].x=BIT_X;
				position[rotation].y=BIT_Y;
				position[rotation].rotation=rotation;
				position[rotation].score=totalScore;
			}
		}//BIT_X--;

		rotateBlock();
	}
	highestScore=0;

	for(rotation=0;rotation<4;rotation++){
		if(position[rotation].score>highestScore){
			highestScore=position[rotation].score;
			bestRotation=rotation;
		}
	}

	BIT_X=firstX;BIT_Y=firstY;
	return position[bestRotation];
}
