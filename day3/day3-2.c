#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


typedef struct MultOp_t_ {
    int n1;
    int n2;
    struct MultOp_t_ *Next;
} MultOp_t;

MultOp_t Program;


int RunProgram(void)
{
    int Sum = 0;
    MultOp_t *op = Program.Next;

    while (op) {
        Sum += (op->n1 * op->n2);
        op = op->Next;
    }
    
    return Sum;
}


void AddMultOp(int n1, int n2)
{
    MultOp_t *op = &Program;
    MultOp_t *end;

    while (op) {
        end = op;
        op = op->Next;
    }

    op = malloc(sizeof(MultOp_t));
    op->n1 = n1;
    op->n2 = n2;
    op->Next = NULL;
    end->Next = op;
}


void ReadProgram(char *filename)
{
    //-------------------
    FILE *fp;
    int c;
    char debug_c;
    int debug_cnt;
    char debug_buffer[7 + 3 + 3];
    enum {
        START,
        MUL_U,
        MUL_L,
        MUL_OPEN_PARENS,
        MUL_DIGIT_1,
        MUL_DIGIT_2,

        DO_DONT_START,
        DO_DONT_DECIDE,

        DONT_APPOSTROPHE,
        DONT_T,
        DONT_OPEN_PARENS,
        DONT_CLOSE_PARENS,

        DO_CLOSE_PARENS,

    } state = START;
    int n1;
    int n2;
    char digit[4];
    int digit_cnt;
    int mul_enabled = 1;
    //-------------------

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed opening %s - %s\n", filename, strerror(errno));
        exit (-1);
    }

    while ((c = fgetc(fp)) != EOF) {

        debug_c = (char)c;

        switch (state) {

            case START:
                debug_cnt = 0;
                memset(debug_buffer, 0, sizeof(debug_buffer));
                debug_buffer[debug_cnt++] = c;

                if (c == 'm') {
                    state = MUL_U;
                }
                else if (c == 'd') {
                    state = DO_DONT_START;
                }
                break;

            case DO_DONT_START:
                if (c == 'o') {
                    debug_buffer[debug_cnt++] = c;
                    state = DO_DONT_DECIDE;
                }
                else {
                    state = START;
                }
                break;

            case DO_DONT_DECIDE:
                if (c == 'n') {
                    debug_buffer[debug_cnt++] = c;
                    state = DONT_APPOSTROPHE;
                }
                else if (c == '(') {
                    debug_buffer[debug_cnt++] = c;
                    state = DO_CLOSE_PARENS;
                }
                else {
                    state = START;
                }
                break;

            case DO_CLOSE_PARENS:
                if (c == ')') {
                    debug_buffer[debug_cnt++] = c;
                    mul_enabled = 1;
                }
                state = START;
                break;

            case DONT_APPOSTROPHE:
                if (c == '\'') {
                    debug_buffer[debug_cnt++] = c;
                    state = DONT_T;
                }
                else {
                    state = START;
                }
                break;

            case DONT_T:
                if (c == 't') {
                    debug_buffer[debug_cnt++] = c;
                    state = DONT_OPEN_PARENS;
                }
                else {
                    state = START;
                }
                break;

            case DONT_OPEN_PARENS:
                if (c == '(') {
                    debug_buffer[debug_cnt++] = c;
                    state = DONT_CLOSE_PARENS;
                }
                else {
                    state = START;
                }
                break;

            case DONT_CLOSE_PARENS:
                if (c == ')') {
                    debug_buffer[debug_cnt++] = c;
                    mul_enabled = 0;
                }
                state = START;
                break;

            case MUL_U:
                if (c == 'u') {
                    debug_buffer[debug_cnt++] = c;
                    state = MUL_L;
                }
                else {
                    state = START;
                }
                break;

            case MUL_L:
                if (c == 'l') {
                    debug_buffer[debug_cnt++] = c;
                    state = MUL_OPEN_PARENS;
                }
                else {
                    state = START;
                }
                break;

            case MUL_OPEN_PARENS:
                if (c == '(') {
                    debug_buffer[debug_cnt++] = c;
                    state = MUL_DIGIT_1;
                    memset(digit, 0, sizeof(digit));
                    digit_cnt = 0;
                    debug_cnt = 4;
                }
                else {
                    state = START;
                }
                break;

            case MUL_DIGIT_1:
                if (isdigit(c)) {
                    debug_buffer[debug_cnt++] = c;
                    digit[digit_cnt++] = c;
                    if (digit_cnt > 3) {
                        state = START;
                    }
                }
                else if (c == ',') {
                    debug_buffer[debug_cnt++] = c;
                    n1 = atoi(digit);
                    state = MUL_DIGIT_2;
                    memset(digit, 0, sizeof(digit));
                    digit_cnt = 0;
                }
                else {
                    state = START;
                }
                break;

            case MUL_DIGIT_2:
                if (isdigit(c)) {
                    debug_buffer[debug_cnt++] = c;
                    digit[digit_cnt++] = c;
                    if (digit_cnt > 3) {
                        state = START;
                    }
                }
                else if (c == ')') {
                    debug_buffer[debug_cnt++] = c;
                    n2 = atoi(digit);
                    if (mul_enabled) {
                        AddMultOp(n1, n2);
                    }
                    //printf("DEBUG: %s = Parsed(%d, %d)\n", debug_buffer, n1, n2);
                    //printf("%s\n", debug_buffer);
                    state = START;
                }
                else {
                    state = START;
                }
                break;

            default:
                printf("State machine broke!\n");
                exit (-1);
        }
    }
}


void Usage(void)
{
    printf("day3 <filename>\n");
    exit(-1);
}


int main (int argc, char *argv[])
{
    //------------------
    int Sum;
    //------------------

    if (argc < 2) {
        Usage();
    }

    ReadProgram(argv[1]);
    Sum = RunProgram();

    printf("Program Output = %d\n", Sum);

    return 0;
}
