#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(void){
	int score=100;
	char scr[10];

	FILE *fp=fopen("./score.txt","w");

	convertInt_Char();
	fputs(scr, fp);

	return 0;
}
