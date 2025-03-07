#include"..\header\print.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    char* outName = "out.txt";
    int flagTermnal = 0;
    int flagBinary = 0;
    char c;
    while ((c = getopt (argc, argv, "tbo:")) != -1){
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
    }
    char* inName = (optind < argc ) ? argv[optind] : NULL;
    if(!inName){
        fprintf(stderr, "[!] brak pliku wejsciowego\n");
        return 1;
    }
    int cutNumber = (optind+1 < argc ) ? atoi(argv[optind+1]) : 1;
    // zapisuje jako liczbe całkowitą 
    int maxMargin = (optind+2 < argc ) ? atoi(argv[optind+2]) : 10;
    if(maxMargin < 0) maxMargin = 0;
    else if(maxMargin > 100) maxMargin = 100;

    // printf("o: %s t: %d b: %d\n", outName, flagTermnal, flagBinary);
    // printf("o: %s t: %d b: %d\n%s %d %d\n", outName, flagTermnal, flagBinary, inName, cutNumber, maxMargin);

    // prit();
    return 0;
}
