#include <stdio.h>
#include <time.h>
#include <stdlib.h>
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

const int SPEED = 1000;
const int AISPEED = 0;//25000 , 2000
int INDEX = 0;
int AIINDEX = 0;
int SCORE = 0;
char COLOUR[7][4]={
    "M",
    "#",
    "$",
    "%",
    "&",
    "K",
    "P"
};
int BOARD_ARR[Y_LEN][X_LEN] = {EMPTY,};
const int _BLOCK_ARR[7][4][4] = {
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

int BLOCK_ARR[4][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

int BIT_X = 4,BIT_Y = 1;
int BIT_Z = 0;

typedef struct position{
    int x, y;
    int rotation;
    int score;
} Pos;

enum { LEFT_KEY=1, DOWN_KEY, RIGHT_KEY, UP_KEY,QUIT, SPACE_KEY };

void gotoXY(int x, int y);
int writeBlockOnBoard(void);
void eraseBlockOffBoard(void);
void gotoXY(int x, int y);
void initGame(void);
void drawBoarder(void);
void drawInsideGame(void);
void rotateBlock(void);
int collision(void);
void stack(void);
void gravity(int y);
int clearLine(void);
void indexCheck(void);
void randomNextBlock(void);
int getIdealPoint(void);
Pos findIdealPosition(void);
int getClearLinePoint(void);

void animationeffect(double par) {
    writeBlockOnBoard();
    drawInsideGame();
    eraseBlockOffBoard();
    //usleep(AISPEED*par);
}

void moveBestPos(Pos bestPosition, char* nxt) {
    BIT_Y = 1;BIT_X = 5;
    for(int i = 0; i < bestPosition.rotation; i++) {
        rotateBlock();
        if(collision())
            *nxt=QUIT;
        animationeffect(7);
    }
    while(BIT_X!=bestPosition.x) {
        if(BIT_X>bestPosition.x) {
            BIT_X--;    
            if(collision())
                *nxt = QUIT;animationeffect(4);
        }
        else if(BIT_X<bestPosition.x) {
            BIT_X++;
            if(collision())
                *nxt = QUIT;animationeffect(4);
        }
    }
    animationeffect(4);
    while(!collision()) {
        animationeffect(0.5);
        BIT_Y++;
    }
    stack();
    return;
}

int main(void) {
    char NXT = '\0';
    Pos bestPosition;
    initGame();
    drawBoarder();
    randomNextBlock();

    while(NXT != QUIT) {
        bestPosition = findIdealPosition();
        moveBestPos(bestPosition, &NXT);
        writeBlockOnBoard();
        drawInsideGame();
        eraseBlockOffBoard();
        indexCheck();
    }
    printf("You Scored %d!\n", SCORE);
    return 0;
}

int writeBlockOnBoard(void) {
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            if(BLOCK_ARR[y][x] != EMPTY) {
                BOARD_ARR[BIT_Y+y][BIT_X+x] = BIT_Z+1;
            }
        }
    }
    return 0;
}

void eraseBlockOffBoard(void) {
    for(int y = 0; y < 4;y++) {
        for(int x = 0; x < 4;x++) {
            if(BLOCK_ARR[y][x] != EMPTY) {
                BOARD_ARR[y + BIT_Y][x + BIT_X] = EMPTY;
            }
        }
    }
}

void gotoXY(int x, int y) {
    printf("\033[%dd\033[%dG", y, x);
}

void initGame(void) {
    printf("\x1b[2J");
    srand((unsigned int)time(NULL));
    for(int x = 0; x < X_RIGHT_BOARDER - X_LEFT_BOARDER; x++) {
        BOARD_ARR[0][x] = LINE;
        BOARD_ARR[Y_LEN - 1][x] = LINE;
    }
    for(int y = 0; y < Y_BOT_BOARDER - Y_TOP_BOARDER; y++) {
        BOARD_ARR[y][0] = LINE;
        BOARD_ARR[y][X_LEN - 1] = LINE;
    }
    gotoXY(40, INDEX++); printf("debug");
}

void drawBoarder(void) {
    for(int x=0, k=0; x < X_LEN; x++, k++) {
        for(int y = 0; y < Y_LEN; y++) {
            if(BOARD_ARR[y][x] == LINE) {
                gotoXY(k + x + X_LEFT_BOARDER, y + Y_TOP_BOARDER); printf("▤");
            }
        }
    }
    gotoXY(4,2); printf("https://github.com/gomjellie");
}

void rotateBlock(void) {
    char tmpArr[4][4];
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            tmpArr[y][x] = BLOCK_ARR[y][x];
        }
    }
    for(int y=0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            BLOCK_ARR[x][3 - y] = tmpArr[y][x];
        }
    }
}

int collision(void) {
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            if(BLOCK_ARR[y][x]!=EMPTY) {
                if(BOARD_ARR[BIT_Y+y][BIT_X+x]!=EMPTY) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void stack(void) {
    gotoXY(40,INDEX++);printf("stack           ");
    BIT_Y--;
    writeBlockOnBoard();
    randomNextBlock();
    BIT_Y = 1; BIT_X = 4;
    clearLine();
}

void gravity(int y) {
    for (;y > 1; y--) {
        for (int x = 1; x < X_LEN - 1; x++) {
            BOARD_ARR[y][x] = BOARD_ARR[y - 1][x];
        }
    }
}

int clearLine(void) {
    int cnt = 0;
    for(int y = Y_LEN - 2; y > 1; y--) {
        cnt = 0;
        for(int x = 1; x < X_LEN - 1; x++) {
            if(BOARD_ARR[y][x] != EMPTY)
                cnt++;
        }
        if(cnt == X_LEN - 2) {
            SCORE += 10;
            gotoXY(40, INDEX++);
            printf("SCORE: %d", SCORE);

            gravity(y);
            y = Y_LEN - 1;
        }
    }
    return 0;
}

void drawInsideGame(void) {
    for(int y = 1; y < Y_LEN - 1; y++) {
        for(int x = 1, k = 1; x < X_LEN - 1; x++, k++) {
            if(BOARD_ARR[y][x] == EMPTY) {
                gotoXY(k + x + X_LEFT_BOARDER, y + Y_TOP_BOARDER);
                printf(" ");
            }
            else if(BOARD_ARR[y][x] != 0) {
                gotoXY(k + x + X_LEFT_BOARDER, y + Y_TOP_BOARDER);
                printf("%s", COLOUR[BOARD_ARR[y][x] - 1]);
            }
        }
    }
}

void indexCheck(void) {
    if(INDEX > 30) {
        for(int i = 0; i <= 33; i++) {
            gotoXY(40, i);
            printf("            ");
        }
        INDEX=0;
    }
    if(AIINDEX > 30) {
        for(int i = 0; i <= 34; i++) {
            gotoXY(60, i);
            printf("           ");
        }
        AIINDEX = 0;
    }
}

void randomNextBlock(void) {
    int prevBIT_Z=BIT_Z;
    while(prevBIT_Z == (BIT_Z = rand() % 7));
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            BLOCK_ARR[y][x] = _BLOCK_ARR[BIT_Z][y][x];
        }
    }
}

int getIdealPoint(void) {
    int boardIdealPoint = 0;
    for(int y = 2; y < Y_LEN - 1; y++) {
        for(int x = 1; x < X_LEN - 1; x++) {
            if(BOARD_ARR[y][x] != EMPTY) {
                boardIdealPoint += y;
            }
        }
    }
    
    return boardIdealPoint;
}

int getAdjacentPoint(void) {
    int adjacentPoint = 0;
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(BLOCK_ARR[y][x] != EMPTY) {
                if(BOARD_ARR[BIT_Y + y][BIT_X + x - 1] != EMPTY) {
                    adjacentPoint += BIT_Y + y;
                }
                if(BOARD_ARR[BIT_Y + y][BIT_X + x + 1] != EMPTY)
                    adjacentPoint += BIT_Y + y;
            }
        }
    }
    return adjacentPoint;
}

int getMinusPoint(void) {
    int boardMinusPoint = 0;
    int height = 0;
    
    for(int x = 1; x < X_LEN - 1; x++) {
        for(int y = 2; BOARD_ARR[y][x] == EMPTY; y++) {
            height = y;
        }
        for(int y = Y_LEN - 1; y > height; y--) {
            if(BOARD_ARR[y][x] == EMPTY) {
                boardMinusPoint += y;
            }
        }
    }
    return boardMinusPoint;
}

int getClearLinePoint(void) {
    int clearLinePoint = 0;
    int count = 0;
    for(int y = BIT_Y; y < BIT_Y + 4; y++) {
        for(int x = 1, count = 0; x < X_LEN - 1; x++) {
            if(BOARD_ARR[y][x] != EMPTY) {
                count++;
            }
        }

        if(count == X_LEN - 2) {
            clearLinePoint += y * (X_LEN - 2);
        }
    }
    
    return clearLinePoint;
}

Pos findIdealPosition(void) {
    int firstX = BIT_X, firstY = BIT_Y;
    Pos position[4] = {0, };
    int highestScore = 0, plusScore = 0, minusScore = 0, totalScore = 0;
    int bestRotation = 0;
    int i = 0;

    for(int rotation=0;rotation<4;rotation++) {
        for(BIT_X = 0, i=0; 1; BIT_X++) {
            BIT_Y = 1;

            if(i == 0&& collision()) {
                BIT_X = 1;
                i++;
            }
            else if(collision()) {
                break;
            }
            while(!collision()) {
                BIT_Y++;
            }
            BIT_Y--;
            writeBlockOnBoard();
            plusScore = 4 * getIdealPoint() + 5 * getClearLinePoint() + 5 * getAdjacentPoint();
            minusScore = 16 * getMinusPoint();
            totalScore = plusScore - minusScore;
            eraseBlockOffBoard();

            if(highestScore <= totalScore) {
                highestScore = totalScore;
                position[rotation].x = BIT_X;
                position[rotation].y = BIT_Y;
                position[rotation].rotation = rotation;
                position[rotation].score = totalScore;
            }
        }

        rotateBlock();
    }
    highestScore = 0;

    for(int rotation = 0; rotation < 4; rotation++) {
        if(position[rotation].score > highestScore) {
            highestScore = position[rotation].score;
            bestRotation = rotation;
        }
    }

    BIT_X = firstX;
    BIT_Y = firstY;
    return position[bestRotation];
}

