#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_REPORTS 2000
int NumReports = 0;

typedef struct Report_t_ {
    int NumLevels;
    int *Report;
    int IsSafe;
}Report_t;

Report_t Reports[MAX_REPORTS];


void ReadReport(char *filename)
{
    //-------------------
    FILE *fp;
    char buffer[256];
    char *p;
    int num_allocated;
    Report_t *rpt;
    //-------------------

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed opening %s - %s\n", filename, strerror(errno));
        exit (-1);
    }

    while(1) {

        if (buffer == fgets(buffer, sizeof(buffer), fp)) {
            
            rpt = &Reports[NumReports];
            num_allocated = 5;
            rpt->NumLevels = 0;
            rpt->Report = malloc(num_allocated * sizeof(int));

            p = strtok(buffer, " \t\n");
            rpt->Report[rpt->NumLevels] = atoi(p);
            if (rpt->Report[rpt->NumLevels] < 0) {
                printf("Error conferting location input %s\n", p);
            }
            rpt->NumLevels++;

            while ((p = strtok(NULL, " \t\n")) != NULL) {
    
                rpt->Report[rpt->NumLevels] = atoi(p);
                if (rpt->Report[rpt->NumLevels] < 0) {
                    printf("Error conferting location input %s\n", p);
                }
                rpt->NumLevels++;

                if (rpt->NumLevels >= num_allocated) {
                    num_allocated *= 2;
                    rpt->Report = realloc(rpt->Report, num_allocated * sizeof(int));
                    if (!rpt->Report) {
                        printf("Failed to realloc\n");
                    }
                }
            }
            
            NumReports++;
            if (NumReports >= (MAX_REPORTS - 1)) {
                printf("Too many locations found!\n");
                exit (-1);
            }
        }
        //
        //  Else we are done with the input file.
        //
        else {
            break;
        }
    }
}


void Usage(void)
{
    printf("day1 <filename>\n");
    exit(-1);
}


void CalcReportSafety(Report_t *rpt)
{
    //------------------------
    enum {
        INCREASING,
        DECREASING
    } change_dir;

    int n1 = rpt->Report[0];
    int n2 = rpt->Report[1];
    //------------------------

    if (n1 < n2) {
        change_dir = INCREASING;
    }
    else if (n1 > n2) {
        change_dir = DECREASING;
    }
    else {
        rpt->IsSafe = 0;
        return;
    }

    for (int i = 1; i < rpt->NumLevels; i++) {

        n2 = rpt->Report[i];

        switch(change_dir) {

            case INCREASING:
                if (n1 > n2) {
                    rpt->IsSafe = 0;
                    return;
                }
                else if ((n2 - n1) > 3){
                    rpt->IsSafe = 0;
                    return;
                }
                else if (n1 == n2) {
                    rpt->IsSafe = 0;
                    return;
                }
                break;

            case DECREASING:
                if (n1 < n2) {
                    rpt->IsSafe = 0;
                    return;
                }
                else if ((n1 - n2) > 3){
                    rpt->IsSafe = 0;
                    return;
                }
                else if (n1 == n2) {
                    rpt->IsSafe = 0;
                    return;
                }
                break;
        }

        n1 = n2;
    }

    rpt->IsSafe = 1;
}


int CalcSafety(void)
{
    //-------------------
    Report_t *rpt;
    int num_safe = 0;
    //-------------------

    for (int i = 0; i < NumReports; i++) {
        CalcReportSafety(&Reports[i]);
        num_safe += Reports[i].IsSafe;
    }

    return num_safe;
}


int main (int argc, char *argv[])
{
    //------------------
    int rc;
    int NumSafe;
    //------------------

    if (argc < 2) {
        Usage();
    }

    ReadReport(argv[1]);

    NumSafe = CalcSafety();

    printf("Number of Reports Safe: %d\n", NumSafe);
    
    return 0;
}