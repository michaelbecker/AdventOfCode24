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
    int Dampener;
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
            rpt->Dampener = 1;
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


int CalcReportSafety(Report_t *rpt)
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
        return 0;
    }

    for (int i = 1; i < rpt->NumLevels; i++) {

        n2 = rpt->Report[i];

        switch(change_dir) {

            case INCREASING:
                if (n1 > n2) {
                    return 0;
                }
                else if ((n2 - n1) > 3){
                    return 0;
                }
                else if (n1 == n2) {
                    return 0;
                }
                break;

            case DECREASING:
                if (n1 < n2) {
                    return 0;
                }
                else if ((n1 - n2) > 3){
                    return 0;
                }
                else if (n1 == n2) {
                    return 0;
                }
                break;
        }

        n1 = n2;
    }

    return 1;
}


Report_t *FilterReport(Report_t *rpt, int exclude_idx)
{
    //--------------------
    Report_t *new_rpt;
    int new_idx = 0;
    //--------------------

    new_rpt = malloc(sizeof(Report_t));
    memset(new_rpt, 0, sizeof(Report_t));
    new_rpt->NumLevels = rpt->NumLevels - 1;
    new_rpt->Report = malloc(sizeof(int) * new_rpt->NumLevels);
    for (int i = 0; i < rpt->NumLevels; i++) {
        if (i == exclude_idx)
            continue;
        new_rpt->Report[new_idx] = rpt->Report[i];
        new_idx++;
    }
    return new_rpt;
}


void FreeFilterReport(Report_t *r)
{
    free(r->Report);
    free(r);
}


int CalcReportSafetyWithDampener(Report_t *rpt)
{
    int safe = CalcReportSafety(rpt);

    if (safe)
        return safe;

    while (rpt->Dampener){
        rpt->Dampener--;

        for (int i = 0; i < rpt->NumLevels; i++) {
            Report_t *rpt_reduced = FilterReport(rpt, i);
            safe = CalcReportSafety(rpt_reduced);
            FreeFilterReport(rpt_reduced);
            if (safe)
                return safe;
        }
    }

    return 0;
}


int CalcSafety(void)
{
    //-------------------
    Report_t *rpt;
    int num_safe = 0;
    //-------------------

    for (int i = 0; i < NumReports; i++) {
        Reports[i].IsSafe = CalcReportSafetyWithDampener(&Reports[i]);
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