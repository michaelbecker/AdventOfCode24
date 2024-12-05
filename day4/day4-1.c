#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define NUM_ROWS 140
#define NUM_COLS 140

//#define NUM_ROWS 10
//#define NUM_COLS 10


char Board[NUM_ROWS][NUM_COLS + 2];
char FoundBoard[NUM_ROWS][NUM_COLS + 2];


void PrintFoundBoard(void)
{
    for (int row = 0; row < NUM_ROWS; row++) {
        printf("%s\n", (char *)&FoundBoard[row]);
    }
}


void IntFoundBoard(void)
{
    for (int row = 0; row < NUM_ROWS; row++) {
        memset(&FoundBoard[row], '.', NUM_COLS);
    }
}


void ReadBoard(char *filename)
{
    //-------------------
    FILE *fp;
    char *p;
    int num_allocated;
    char *buffer;
    //-------------------

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed opening %s - %s\n", filename, strerror(errno));
        exit (-1);
    }

    for (int i = 0; i < NUM_ROWS; i++) {

        buffer = &Board[i][0];        

        if (buffer != fgets(buffer, NUM_COLS + 2, fp)) {
            printf("Failed parsing input file!\n");
            exit(-1);
        }
    }
}


int CheckForward(int row, int col)
{
    if ((Board[row][col] == 'X') &&
        (Board[row][col + 1] == 'M') &&
        (Board[row][col + 2] == 'A') &&
        (Board[row][col + 3] == 'S')) {
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row][col + 1] = Board[row][col + 1];
        FoundBoard[row][col + 2] = Board[row][col + 2];
        FoundBoard[row][col + 3] = Board[row][col + 3];
        return 1;
    }
    else
        return 0;
}


int CountForward(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS - 3; col++){
            Count += CheckForward(row, col);
        }
    }
    printf("CountForward = %d\n", Count);
    return Count;
}


int CheckBackwards(int row, int col)
{
    if ((Board[row][col] == 'S') &&
        (Board[row][col + 1] == 'A') &&
        (Board[row][col + 2] == 'M') &&
        (Board[row][col + 3] == 'X')) {
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row][col + 1] = Board[row][col + 1];
        FoundBoard[row][col + 2] = Board[row][col + 2];
        FoundBoard[row][col + 3] = Board[row][col + 3];
        return 1;
    }
    else
        return 0;
}


int CountBackwards(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS - 3; col++){
            Count += CheckBackwards(row, col);
        }
    }
    printf("CountBackwards = %d\n", Count);
    return Count;
}


int CheckVerticaly(int row, int col)
{
    if ((Board[row][col] == 'X') &&
        (Board[row + 1][col] == 'M') &&
        (Board[row + 2][col] == 'A') &&
        (Board[row + 3][col] == 'S')) {
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row + 1][col] = Board[row + 1][col];
        FoundBoard[row + 2][col] = Board[row + 2][col];
        FoundBoard[row + 3][col] = Board[row + 3][col];
        return 1;
    }
    else
        return 0;
}


int CountVerticaly(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS - 3; row++) {
        for (int col = 0; col < NUM_COLS; col++){
            Count += CheckVerticaly(row, col);
        }
    }
    printf("CountVerticaly = %d\n", Count);
    return Count;
}


int CheckVerticalyBackwards(int row, int col)
{
    if ((Board[row][col] == 'S') &&
        (Board[row + 1][col] == 'A') &&
        (Board[row + 2][col] == 'M') &&
        (Board[row + 3][col] == 'X')){
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row + 1][col] = Board[row + 1][col];
        FoundBoard[row + 2][col] = Board[row + 2][col];
        FoundBoard[row + 3][col] = Board[row + 3][col];
        return 1;
    }
    else
        return 0;
}


int CountVerticalyBackwards(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS - 3; row++) {
        for (int col = 0; col < NUM_COLS; col++){
            Count += CheckVerticalyBackwards(row, col);
        }
    }
    printf("CountVerticalyBackwards = %d\n", Count);
    return Count;
}


int CheckDiagonal(int row, int col)
{
    if ((Board[row][col] == 'X') &&
        (Board[row + 1][col + 1] == 'M') &&
        (Board[row + 2][col + 2] == 'A') &&
        (Board[row + 3][col + 3] == 'S')){
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row + 1][col + 1] = Board[row + 1][col + 1];
        FoundBoard[row + 2][col + 2] = Board[row + 2][col + 2];
        FoundBoard[row + 3][col + 3] = Board[row + 3][col + 3];
        return 1;
    }
    else
        return 0;
}


int CountDiagonal(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS - 3; row++) {
        for (int col = 0; col < NUM_COLS - 3; col++){
            Count += CheckDiagonal(row, col);
        }
    }
    printf("CountDiagonal = %d\n", Count);
    return Count;
}


int CheckDiagonalBackwards(int row, int col)
{
    if ((Board[row][col] == 'S') &&
        (Board[row + 1][col + 1] == 'A') &&
        (Board[row + 2][col + 2] == 'M') &&
        (Board[row + 3][col + 3] == 'X')){
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row + 1][col + 1] = Board[row + 1][col + 1];
        FoundBoard[row + 2][col + 2] = Board[row + 2][col + 2];
        FoundBoard[row + 3][col + 3] = Board[row + 3][col + 3];
        return 1;
    }
    else
        return 0;
}


int CountDiagonalBackwards(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS - 3; row++) {
        for (int col = 0; col < NUM_COLS - 3; col++){
            Count += CheckDiagonalBackwards(row, col);
        }
    }
    printf("CountDiagonalBackwards = %d\n", Count);
    return Count;
}


int CheckDiagonalReverse(int row, int col)
{
    if ((Board[row][col] == 'X') &&
        (Board[row + 1][col - 1] == 'M') &&
        (Board[row + 2][col - 2] == 'A') &&
        (Board[row + 3][col - 3] == 'S')){
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row + 1][col - 1] = Board[row + 1][col - 1];
        FoundBoard[row + 2][col - 2] = Board[row + 2][col - 2];
        FoundBoard[row + 3][col - 3] = Board[row + 3][col - 3];
        return 1;
    }
    else
        return 0;
}


int CountDiagonalReverse(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS - 3; row++) {
        for (int col = 3; col < NUM_COLS; col++){
            Count += CheckDiagonalReverse(row, col);
        }
    }
    printf("CountDiagonalReverse = %d\n", Count);
    return Count;
}


int CheckDiagonalReverseBackwards(int row, int col)
{
    if ((Board[row][col] == 'S') &&
        (Board[row + 1][col - 1] == 'A') &&
        (Board[row + 2][col - 2] == 'M') &&
        (Board[row + 3][col - 3] == 'X')){
        FoundBoard[row][col] = Board[row][col];
        FoundBoard[row + 1][col - 1] = Board[row + 1][col - 1];
        FoundBoard[row + 2][col - 2] = Board[row + 2][col - 2];
        FoundBoard[row + 3][col - 3] = Board[row + 3][col - 3];
        return 1;
    }
    else
        return 0;
}


int CountDiagonalReverseBackwards(void)
{
    int Count = 0;
    for (int row = 0; row < NUM_ROWS - 3; row++) {
        for (int col = 3; col < NUM_COLS; col++){
            Count += CheckDiagonalReverseBackwards(row, col);
        }
    }
    printf("CountDiagonalReverse = %d\n", Count);
    return Count;
}



int CountNumXmas(void)
{
    int Count = 0;
    Count += CountForward();
    Count += CountBackwards();
    Count += CountVerticaly();
    Count += CountVerticalyBackwards();
    Count += CountDiagonal();
    Count += CountDiagonalBackwards();
    Count += CountDiagonalReverse();
    Count += CountDiagonalReverseBackwards();

    return Count;
}



void Usage(void)
{
    printf("day1 <filename>\n");
    exit(-1);
}


int main (int argc, char *argv[])
{
    //------------------
    int rc;
    int Count;
    //------------------

    if (argc < 2) {
        Usage();
    }

    IntFoundBoard();

    ReadBoard(argv[1]);

    Count = CountNumXmas();

    PrintFoundBoard();

    printf("Number XMAS: %d\n", Count);
    
    return 0;
}
