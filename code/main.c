#include"..\header\print.h"
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

    // prit();
    return 0;
}
