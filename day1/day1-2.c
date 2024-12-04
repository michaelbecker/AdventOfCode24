#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_LOCATIONS 2000
int NumLocations = 0;
int Location1[MAX_LOCATIONS];
int Location2[MAX_LOCATIONS];


void ReadInput(char *filename)
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
            Location1[NumLocations] = atoi(p);
            if (Location1[NumLocations] < 0) {
                printf("Error conferting location input %s\n", p);
            }

            p = strtok(NULL, " \t\n");
            Location2[NumLocations] = atoi(p);
            if (Location2[NumLocations] < 0) {
                printf("Error conferting location input %s\n", p);
            }
            
            NumLocations++;
            if (NumLocations >= (MAX_LOCATIONS - 1)) {
                printf("Too many locations found!\n");
                exit (-1);
            }
        }
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


int CalcSimilarity(void)
{
    int Similarity = 0;

    for (int i = 0; i < NumLocations; i++) {

        int count = 0;
        
        for (int j = 0; j < NumLocations; j++) {
            if (Location1[i] == Location2[j]) {
                count++;
            }
        }

        Similarity += (count * Location1[i]);
    }

    return Similarity;
}


int main (int argc, char *argv[])
{
    //------------------
    int rc;
    int Similarity;
    //------------------

    if (argc < 2) {
        Usage();
    }

    ReadInput(argv[1]);

    Similarity = CalcSimilarity();

    printf("Similarity between lists? Ans: %d\n", Similarity);
    
    return 0;
}