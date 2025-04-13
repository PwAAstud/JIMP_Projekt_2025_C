#include<stdio.h>
#include"..\header\grafCutComb.h"

void printSet(char* set, int n){
    for(int i=0; i<n; i++){
        printf("%d ", set[i]);
    }
    printf("\n");
}

void makeSet(char* set, int n, int size){
    int i;
    for(i = 0; i<size; i++){
        set[i] = 1;
    }
    for(; i<n; i++){
        set[i] = 0;
    }
}

int setNext(char* set, int n){
    int i;
    for(i = n-1; !set[i] && i>=0; i--);
    if(i<n-1){
        set[i] = 0; 
        set[i+1] = 1;
        return 1;
    }

    int inFront = 2;
    set[n-1] = 0;
    for(i = n-2; set[i] && i>=0; i--){
        set[i] = 0;
        inFront++;
    }
    for(; !set[i] && i>=0; i--);
    if(i<0){
        return 0;
    }
    set[i] = 0;
    for(; inFront > 0; inFront--){
        set[i+inFront] = 1;
    }
    return 1;
}

// nie usuwa polaczen miedzy outA i outB
// outA to 1
// outB to 0
void static makeSubGrafs(node** graf, char * set, long numGrafSet, node** outA, node** outB){
    int numA=0, numB=0;
    for(int i=0; i < numGrafSet; i++){
        if(set[i]){
            outA[numA] = graf[i];
            numA++;
        }else{
            outB[numB] = graf[i];
            numB++;
        }
    }
}

long cutGrafComb(node** graf, long numGraf, int margin, node*** out){
    if(numGraf <= 1){
        return 0;
    }

    long maxSize = numGraf*(200+margin)/400;
    if(maxSize >= numGraf){
        maxSize = numGraf - 1;
    }
    long minSize = numGraf - maxSize;
    // printf("%ld - %ld\n", minSize, maxSize);
    if(maxSize < minSize){
        return 0;
    }

    sortGrafData(graf, numGraf);

    struct{
        long nodeIds[numGraf];
        long n;
        long cut;
    } bestCut;
    bestCut.n = 0;
    bestCut.cut = 0x7fffffff; // licze ze nie bedzie tak dzurzego ciencia. potrebuje liczby inf na start

    char set[numGraf];
    node* grafA[numGraf];
    node* grafB[numGraf];

    // long iteration =0;

    for(int setSize = minSize; setSize<=numGraf/2; setSize++){
        // printf("\r%d",setSize);
        int numGrafA = setSize;
        int numGrafB = numGraf - setSize;
        // printf("%d\n", setSize);
        makeSet(set, numGraf, setSize);
        do{
            // printf("\r%ld", iteration++);
            makeSubGrafs(graf, set, numGraf, grafA, grafB);
            if(!isGrafConected(grafA, numGrafA) || !isGrafConected(grafB, numGrafB)){
                continue;
            }
            long newCut = countCuts(grafA, numGrafA);
            // printf("\nA: ");
            // for(int i=0; i<numGrafA; i++){
            //     printf("%ld ", grafA[i]->id);
            // }
            // printf("; B: ");
            // for(int i=0; i<numGrafB; i++){
            //     printf("%ld ", grafB[i]->id);
            // }
            // // printf("; %ld", newCut);
            // printf("\n");

            if(newCut < bestCut.cut){
                for(long i=0; i < numGrafA; i++){
                    bestCut.nodeIds[i] = grafA[i]->id;
                }
                bestCut.cut = newCut;
                bestCut.n = numGrafA;
            }
            // break;

        } while(setNext(set, numGraf));
        // if(bestCut.n != 0) break;
    }
    printf("\n ");
    for(int i=0; i<bestCut.n; i++){
        printf("%ld ", bestCut.nodeIds[i]);
    }
    printf("\n");
    returnNewGraf(graf, numGraf, bestCut.nodeIds, bestCut.n, out);

    return bestCut.n;
}
