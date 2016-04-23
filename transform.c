#include <stdio.h>
char arr[4][4]={
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};
void transform(void);
int main(void){
	int i=0,j=0;
	
	transform();

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
	return 0;
}
void transform(void){
	char tmpArr[4][4];
	int x=0,y=0;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			tmpArr[y][x]=arr[y][x];
		}
	}
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			arr[x][3-y]=tmpArr[y][x];
		}
	}
}
