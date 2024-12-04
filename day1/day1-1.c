#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_LOCATIONS 2000
int NumReports = 0;
int Location1[MAX_REPORTS];
int Location2[MAX_REPORTS];
int Diff[MAX_REPORTS];


void ReadReport(char *filename)
{
    //-------------------
    FILE *fp;
    char buffer[256];
    char *p;
    //-------------------

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed opening %s - %s\n", filename, strerror(errno));
        exit (-1);
    }

    while(1) {
        if (buffer == fgets(buffer, sizeof(buffer), fp)) {
            
            p = strtok(buffer, " \t\n");
            Location1[NumReports] = atoi(p);
            if (Location1[NumReports] < 0) {
                printf("Error conferting location input %s\n", p);
            }

            p = strtok(NULL, " \t\n");
            Location2[NumReports] = atoi(p);
            if (Location2[NumReports] < 0) {
                printf("Error conferting location input %s\n", p);
            }
            
            NumReports++;
            if (NumReports >= (MAX_REPORTS - 1)) {
                printf("Too many locations found!\n");
                exit (-1);
            }
        }
        else {
            break;
        }
    }
}


int MyCompare(const void *a, const void *b)
{
    int aa = *(int *)a;
    int bb = *(int *)b;

    return (aa - bb);
}


void Usage(void)
{
    printf("day1 <filename>\n");
    exit(-1);
}


void CalculateDiff(void)
{
    for (int i = 0; i < NumReports; i++) {
        Diff[i] = abs(Location1[i] - Location2[i]);
    }
}


int SumDiffs(void)
{
    int Sum = 0;
    for (int i = 0; i < NumReports; i++) {
        Sum += Diff[i];
    }
    return Sum;
}


int main (int argc, char *argv[])
{
    //------------------
    int rc;
    int Sum;
    //------------------

    if (argc < 2) {
        Usage();
    }

    ReadReport(argv[1]);
    qsort(Location1, NumReports, sizeof(int), MyCompare);
    qsort(Location2, NumReports, sizeof(int), MyCompare);
    CalculateDiff();
    Sum = SumDiffs();

    printf("What is the total distance between your lists? Ans: %d\n", Sum);
    
    return 0;
}