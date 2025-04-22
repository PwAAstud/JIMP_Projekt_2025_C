#include"..\header\print.h"
#include"..\header\grafIO.h"
#include"..\header\grafCutComb.h"
#include"..\header\grafCutStoer.h"
#include"..\header\grafCutGrow.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    char* outName = "out.txt";
    int flagTermnal = 0;
    int flagBinary = 0;
    char c;
    char* noOption[argc];
    int numNoOption = 0;
    while (optind < argc){
        if((c = getopt (argc, argv, "tbo:")) != -1){
            switch (c){
                case 'o':
                    outName = optarg;
                    break;
                case 't':
                    flagTermnal = 1;
                    break;
                case 'b':
                    flagBinary = 1;
                    break;
                default:
                    break;
            }
        }else{
            noOption[numNoOption] = argv[optind];
            numNoOption++;
            optind++;
        }
    }
    char* inName = (numNoOption >= 1) ? noOption[0] : NULL;
    if(!inName){
        fprintf(stderr, "[!] brak pliku wejsciowego\n");
        return 1;
    }
    int cutNumber = (numNoOption >= 2) ? atoi(noOption[1]) : 1;
    // zapisuje jako liczbe całkowitą 
    int maxMargin = (numNoOption >= 3) ? atoi(noOption[2]) : 10;
    if(maxMargin < 0) maxMargin = 0;

    printf("o: %s t: %d b: %d\n", outName, flagTermnal, flagBinary);
    printf("%s %d %d\n", inName, cutNumber, maxMargin);

    grafData* fileData = loadGrafData(inName);

    // printGrafData(fileData);

    node** grafsToCuts[cutNumber+1];
    long numGrafsToCuts[cutNumber+1];
    grafsToCuts[0] = fileData->graf;
    numGrafsToCuts[0] = fileData->numGraf;
    // sortGrafData(grafsToCuts[0], numGrafsToCuts[0]);
    // printf("%d\n", isGrafConected(grafsToCuts[0], numGrafsToCuts[0]));

    long cutGraf;
    long sucesGraf = 1;
    long sucesCut = 0;
    long newLen;
    for(long i = 1; i<=cutNumber; i++){
        newLen = 0;
        for(cutGraf = 0; cutGraf < sucesGraf; cutGraf++){
            if(!isGrafConected(grafsToCuts[cutGraf], numGrafsToCuts[cutGraf])){ continue;}
            newLen = cutGrafStoner(grafsToCuts[cutGraf], numGrafsToCuts[cutGraf], maxMargin, &grafsToCuts[sucesGraf]);
            numGrafsToCuts[cutGraf] -= newLen;
            numGrafsToCuts[sucesGraf] = newLen;
            // printf("%ld %ld\n", cutGraf, sucesGraf);
            // printf("%ld ", newLen);
            if(newLen != 0){
                sucesGraf++;
                // printf("a");
                break;
            }
        }
        // printf("%ld\n", newLen);
        if(newLen == 0){
            break;
        }
    }
    sucesCut = sucesGraf-1;

    if(flagTermnal){
        for(long i=0; i<sucesGraf;i++){
            printf("\n");
            // printf("%ld ", numGrafsToCuts[i]);
            // printf("is conected: %d\n",isGrafConected(grafsToCuts[i], numGrafsToCuts[i]));
            printGraf(grafsToCuts[i], numGrafsToCuts[i]);
        }
    }
    // printf("%ld<", sucesGraf);
    long index = 0;
    for(long i=0; i<sucesGraf;i++){
        // printf("%ld ", numGrafsToCuts[i]);
        for(long j=0; j<numGrafsToCuts[i]; j++){
            // printf("%ld ", index);
            fileData->graf[index] = grafsToCuts[i][j];
            index++;
        }
    }
    // sortGrafData(fileData->graf, fileData->numGraf);
    // printGraf(fileData->graf, fileData->numGraf);
    printf("%ld\n", sucesCut);
    if(!flagBinary){
        saveGrafDataTxt(outName, fileData, sucesCut);
    }else{
        saveGrafDataBin(outName, fileData, sucesCut);
    }
    return 0;
}
