#include"../header/grafCutFiduMatt.h"
#include<stdio.h>

// na bazie https://en.wikipedia.org/wiki/Fiduccia%E2%80%93Mattheyses_algorithm

long static findBigestCost(long* nodeCost, long numCost){
    long retVal = 0;
    for(long i=1; i<numCost; i++){
        if(nodeCost[i] > nodeCost[retVal]){
            retVal = i;
        }
    }
    return retVal;
}

// limitTo czy A'0' czy B'1'
// nie zazac mu szukac po pustym przedziale
long static findBigestCostLimited(char* set, long* nodeCost, long numCost, char limitTo){
    long retVal = 0;
    for(; set[retVal] != limitTo; retVal++);
    for(long i=retVal+1; i<numCost; i++){
        if(set[i] != limitTo){
            continue;
        }
        if(nodeCost[i] > nodeCost[retVal]){
            retVal = i;
        }
    }
    return retVal;
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

// wymaga posortowanego grafu
void static switchNode(node** graf, char* set, long* nodeCost, long numGraf, long nodeIndex){
    for(long i=0; i<graf[nodeIndex]->n; i++){
        long conectionInde = grafBinSearch(graf, numGraf, graf[nodeIndex]->conetion[i]->id);
        if(set[nodeIndex] == set[conectionInde]){
            nodeCost[conectionInde] += 2;
        }else{
            nodeCost[conectionInde] -= 2;
        }
    }
    set[nodeIndex] = !set[nodeIndex];
    nodeCost[nodeIndex] *= -1;
    // nodeCost[nodeIndex] = 0x80000000;
}

long cutGrafFiduMatt(node** graf, long numGraf, int margin, node*** out){
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

    // 0 grafA, 1 grafB
    char set[numGraf];
    long nodeCost[numGraf];
    for(long i=0; i<numGraf; i++){
        set[i] = 0;
        nodeCost[i] = -graf[i]->n;
    }
    node* grafA[numGraf];
    node* grafB[numGraf];
    long numGrafA = numGraf;
    long numGrafB = 0;
    
    long indexToMove;

    // printGraf(graf, numGraf);
    switchNode(graf, set, nodeCost, numGraf, 2);
    numGrafA--;
    numGrafB++;
    switchNode(graf, set, nodeCost, numGraf, 3);
    numGrafA--;
    numGrafB++;
    switchNode(graf, set, nodeCost, numGraf, 4);
    numGrafA--;
    numGrafB++;
    switchNode(graf, set, nodeCost, numGraf, 6);
    numGrafA--;
    numGrafB++;
    // robimu numGraf razu by był absolutny warunek zakonczenia 
    for(long i=0; i<numGraf; i++){
        for(long j=0; j<numGraf; j++){
            printf("(%d %2ld) ", set[j],nodeCost[j]);
        }
        printf("\n");
        printf("%ld %ld; ", numGrafA, numGrafB);
        if(numGrafB < minSize){
            indexToMove = findBigestCostLimited(set, nodeCost, numGraf, 0);
        }else if(numGrafA < minSize){
            indexToMove = findBigestCostLimited(set, nodeCost, numGraf, 1);
        }else{
            indexToMove = findBigestCost(nodeCost, numGraf);
        }
        printf("%ld\n", indexToMove);
        if(set[indexToMove] == 1){
            numGrafA++;
            numGrafB--;
        }else{
            numGrafA--;
            numGrafB++;
        }
        switchNode(graf, set, nodeCost, numGraf, indexToMove);
        // nodeCost[indexToMove] = 0x80000000;
        
        makeSubGrafs(graf, set, numGraf, grafB, grafA);
        if(numGrafB < minSize || numGrafA < minSize){
            continue;
        }
        // printGraf(grafA, numGrafA);
        if(!isGrafConected(grafA, numGrafA) || !isGrafConected(grafB, numGrafB)){
            continue;
        }
        long newCut = 0;
        
        if(newCut < bestCut.cut){
            for(long i=0; i < numGrafB; i++){
                bestCut.nodeIds[i] = grafB[i]->id;
            }
            bestCut.cut = newCut;
            bestCut.n = numGrafB;
        }
    }
    returnNewGraf(graf, numGraf, bestCut.nodeIds, bestCut.n, out);

    return bestCut.n;
}
