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

// wymaga polaczen do nikad
long countCuts(node** graf, int numGraf){
    long voidConection = 0;
    for(long i = 0; i < numGraf; i++){
        int z=0;
        for(int j = 0; j < graf[i]->n; j++){
            for(; z<numGraf && graf[z]->id < graf[i]->conetion[j]->id; z++);
            if(graf[i]->conetion[j] != graf[z]){
                voidConection++;
            }
        }
    }
    return voidConection;
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
    for(int setSize = minSize; setSize<=numGraf/2; setSize++){
        int numGrafA = setSize;
        int numGrafB = numGraf - setSize;

        makeSet(set, numGraf, setSize);
        do{
            makeSubGrafs(graf, set, numGraf, grafA, grafB);
            if(!isGrafConected(grafA, numGrafA) || !isGrafConected(grafB, numGrafB)){
                continue;
            }
            long newCut = countCuts(grafA, numGrafA);
            // printf("A: ");
            // for(int i=0; i<numGrafA; i++){
            //     printf("%ld ", grafA[i]->id);
            // }
            // printf("; B: ");
            // for(int i=0; i<numGrafB; i++){
            //     printf("%ld ", grafB[i]->id);
            // }
            // printf("; %ld", newCut);
            // printf("\n");

            if(newCut < bestCut.cut){
                for(long i=0; i < numGrafA; i++){
                    bestCut.nodeIds[i] = grafA[i]->id;
                }
                bestCut.cut = newCut;
                bestCut.n = numGrafA;
            }

        } while(setNext(set, numGraf));
    }

    returnNewGraf(graf, numGraf, bestCut.nodeIds, bestCut.n, out);

    return bestCut.n;
}
