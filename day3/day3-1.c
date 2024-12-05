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
        GET_M,
        GET_U,
        GET_L,
        GET_OPEN_PARENS,
        GET_DIGIT_1,
        GET_DIGIT_2,
    } state = GET_M;
    int n1;
    int n2;
    char digit[4];
    int digit_cnt;
    //-------------------

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed opening %s - %s\n", filename, strerror(errno));
        exit (-1);
    }

    while ((c = fgetc(fp)) != EOF) {

        debug_c = (char)c;

        switch (state) {

            case GET_M:
                if (c == 'm') {
                    state = GET_U;
                    debug_cnt = 0;
                    memset(debug_buffer, 0, sizeof(debug_buffer));
                    debug_buffer[debug_cnt++] = c;
                }
                break;

            case GET_U:
                if (c == 'u') {
                    debug_buffer[debug_cnt++] = c;
                    state = GET_L;
                }
                else {
                    state = GET_M;
                }
                break;

            case GET_L:
                if (c == 'l') {
                    debug_buffer[debug_cnt++] = c;
                    state = GET_OPEN_PARENS;
                }
                else {
                    state = GET_M;
                }
                break;

            case GET_OPEN_PARENS:
                if (c == '(') {
                    debug_buffer[debug_cnt++] = c;
                    state = GET_DIGIT_1;
                    memset(digit, 0, sizeof(digit));
                    digit_cnt = 0;
                    debug_cnt = 4;
                }
                else {
                    state = GET_M;
                }
                break;

            case GET_DIGIT_1:
                if (isdigit(c)) {
                    debug_buffer[debug_cnt++] = c;
                    digit[digit_cnt++] = c;
                    if (digit_cnt > 3) {
                        state = GET_M;
                    }
                }
                else if (c == ',') {
                    debug_buffer[debug_cnt++] = c;
                    n1 = atoi(digit);
                    state = GET_DIGIT_2;
                    memset(digit, 0, sizeof(digit));
                    digit_cnt = 0;
                }
                else {
                    state = GET_M;
                }
                break;

            case GET_DIGIT_2:
                if (isdigit(c)) {
                    debug_buffer[debug_cnt++] = c;
                    digit[digit_cnt++] = c;
                    if (digit_cnt > 3) {
                        state = GET_M;
                    }
                }
                else if (c == ')') {
                    debug_buffer[debug_cnt++] = c;
                    n2 = atoi(digit);
                    AddMultOp(n1, n2);
                    //printf("DEBUG: %s = Parsed(%d, %d)\n", debug_buffer, n1, n2);
                    //printf("%s\n", debug_buffer);
                    state = GET_M;
                }
                else {
                    state = GET_M;
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