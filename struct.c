#include <stdio.h>
#include <string.h>

typedef struct position{
	int x,y;
	int rotation;
	int score;
}Pos;

Pos function(void){
	Pos position[4]={0,};

	position[3].x=1;
	position[3].y=2;
	position[3].rotation=3;
	position[3].score=4;

	return position[3];
};

int main(void){
	Pos bestPos;
	bestPos=function();

	printf("%d", bestPos.x);

	return 0;
}
