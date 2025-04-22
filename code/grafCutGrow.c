#include"..\header\graf.h"
#include"..\header\grafCutGrow.h"
#include<stdlib.h>
#include<stdio.h>

int static isSet0conected(node** graf, long numGraf, char* set){
    // 0 uncheck
    // 2 check
    // graf 1
    // printf("%d: ", numGraf);
    long emptyNode;
    for(emptyNode=0; emptyNode < numGraf && set[emptyNode] != 0; emptyNode++);
    if(emptyNode == numGraf){
        return 0;
    }

    struct{
        long next[numGraf]; // indexy
        long n;
    } stack;
    stack.n = 1;
    stack.next[0] = emptyNode;

    while (stack.n > 0){
        stack.n--;
        long nodeToCheck = stack.next[stack.n];

        set[nodeToCheck] = 2;
        for(long i = 0; i < graf[nodeToCheck]->n; i++){
            long conPos = grafBinSearch(graf, numGraf, graf[nodeToCheck]->conetion[i]->id);
            if(set[conPos] != 0){
                continue;
            }
            stack.next[stack.n] = conPos;
            stack.n++;
        }
    }

    for(emptyNode=0; emptyNode < numGraf && set[emptyNode] != 0; emptyNode++);
    for(long i=0; i < numGraf; i++){
        // printf("%d ", set[i]);
        if(set[i] != 1){
            set[i] = 0;
        }
    }
    // printf("\n");
    return emptyNode == numGraf;
}

void static sortByWagi(long* wagi, long* sortList, long num){
    for(long i=num; i>0;i--){
        for(long j=1; j<i;j++){
            if(wagi[sortList[j]] < wagi[sortList[j-1]]){
                long temp = sortList[j];
                sortList[j] = sortList[j-1];
                sortList[j-1] = temp;
                // continue;
            }
        }
        // for(long i=0; i<num;i++){
        //     printf("(%d:%d) ", wagi[sortList[i]],sortList[i]);
        // }
        // printf("\n");
    }
}

typedef struct{
    long changeNode; // id
    long* allConected; // numConected 
    long* wagi; // numConected
    long numConected;
    long* untested; // indexy listy allConected i wagi numToTest
    long  numToTest;
} growMem;

static growMem* FistMem(node* nodeToAdd){
    growMem* retVal = malloc(sizeof(growMem));
    if(!retVal){ printf("[!] pamiec");}
    retVal->changeNode = nodeToAdd->id;
    retVal->numConected = nodeToAdd->n;
    retVal->numToTest = nodeToAdd->n;
    retVal->allConected = malloc(sizeof(long)*nodeToAdd->n);
    if(!retVal->allConected){ printf("[!] pamiec");}
    retVal->wagi = malloc(sizeof(long)*nodeToAdd->n);
    if(!retVal->wagi){ printf("[!] pamiec");}
    retVal->untested = malloc(sizeof(long)*nodeToAdd->n);
    if(!retVal->untested){ printf("[!] pamiec");}
    for(long i=0; i< nodeToAdd->n;i++){
        retVal->allConected[i] = nodeToAdd->conetion[i]->id;
        retVal->wagi[i] = 1;
        retVal->untested[i] = i;
    }
    sortByWagi(retVal->wagi, retVal->untested, retVal->numToTest);
    return retVal;
}

static growMem* memPlusNode(growMem* past, node* nodeToAdd, node** graf, long numGraf, char* set){
    growMem* retVal = malloc(sizeof(growMem));
    if(!retVal){ printf("[!] pamiec");}
    retVal->changeNode = nodeToAdd->id;

    long newLen = nodeToAdd->n+past->numConected;
    retVal->allConected = malloc(sizeof(long)*newLen);
    if(!retVal->allConected){ printf("[!] pamiec");}
    retVal->wagi = malloc(sizeof(long)*newLen);
    if(!retVal->wagi){ printf("[!] pamiec");}
    retVal->untested = malloc(sizeof(long)*newLen);
    if(!retVal->untested){ printf("[!] pamiec");}

    newLen = 0;
    long pointerPast = 0, pointerNode = 0;
    while (pointerPast != past->numConected && pointerNode != nodeToAdd->n){
        // if(pointerPast != past->numConected && past->allConected[pointerPast] == nodeToAdd->id){
        //     pointerPast++;
        // }
        // printf("%d %d, ", pointerPast, pointerNode);
        if(past->allConected[pointerPast] <= nodeToAdd->conetion[pointerNode]->id){
            retVal->allConected[newLen] = past->allConected[pointerPast];
            retVal->wagi[newLen] = past->wagi[pointerPast];

            if(pointerNode != nodeToAdd->n && past->allConected[pointerPast] == nodeToAdd->conetion[pointerNode]->id){
                retVal->wagi[newLen]++;
                pointerNode++;
            }
            pointerPast++;
        }else{
            retVal->allConected[newLen] = nodeToAdd->conetion[pointerNode]->id;
            retVal->wagi[newLen] = 1;

            pointerNode++;
        }
        newLen++;
    }
    for(; pointerPast != past->numConected; pointerPast++){
        retVal->allConected[newLen] = past->allConected[pointerPast];
        retVal->wagi[newLen] = past->wagi[pointerPast];
        newLen++;
    }
    for(; pointerNode != nodeToAdd->n; pointerNode++){
        retVal->allConected[newLen] = nodeToAdd->conetion[pointerNode]->id;
        retVal->wagi[newLen] = 1;
        newLen++;
    }
    // printf("%d %d; ", pointerPast, pointerNode);
    long inderCon = 0;
    for(long i=0;i<newLen;i++){
        long idIndex = grafBinSearch(graf, numGraf, retVal->allConected[i]);
        if(set[idIndex] == 1){
            inderCon++;
            continue;
        }
        retVal->allConected[i-inderCon] = retVal->allConected[i];
        retVal->wagi[i-inderCon] = retVal->wagi[i];
    }
    newLen -= inderCon;

    retVal->numConected = newLen;

    for(long i=0;i<newLen;i++){
        retVal->untested[i] = i;
    }
    retVal->numToTest = newLen;

    sortByWagi(retVal->wagi, retVal->untested, retVal->numToTest);
    return retVal;
}

void static memFree(growMem* toFree){
    free(toFree->allConected);
    free(toFree->untested);
    free(toFree->wagi);
    free(toFree);
}

long static grow(node** graf, long numGraf, long minM, long maxM, char* set){
    growMem* stosAkcji[maxM];
    long stosHeight = 0;

    set[0] = 1;
    stosAkcji[0] = FistMem(graf[0]);
    stosHeight++;
    while(stosHeight>0){
        long toUndu;
        
        // for(long i=0;i<numGraf;i++){
            //     printf("%d ", set[i]);
            // }
            // printf("-> ");
            
            stosAkcji[stosHeight-1]->numToTest--;
            long conIndex = stosAkcji[stosHeight-1]->untested[ stosAkcji[stosHeight-1]->numToTest ];
            long nodeAddSet = grafBinSearch(graf, numGraf, stosAkcji[stosHeight-1]->allConected[conIndex]);
            set[nodeAddSet] = 1;
        stosAkcji[stosHeight] = memPlusNode(stosAkcji[stosHeight-1], graf[nodeAddSet], graf, numGraf, set);
        stosHeight++;
        
        // for(long i=0;i<numGraf;i++){
            //     printf("%d ", set[i]);
            // }
            // printf("\n");
            
            // printf("\r");
            // printf("%ld ", stosAkcji[0]->numToTest);
            // for(long i=0;i<40 && i<stosHeight;i++){
                //     printf("%ld ", stosAkcji[i]->numToTest);
                // }
                // printf(";");
                
        if(stosHeight >= minM){
            if(isSet0conected(graf, numGraf, set)){
                return stosHeight;
            }
        }
        while (stosHeight >= maxM){
            toUndu = grafBinSearch(graf, numGraf, stosAkcji[stosHeight-1]->changeNode);
            set[toUndu] = 0;
            stosHeight--;
            memFree(stosAkcji[stosHeight]);
            // printf("\r%ld", stosHeight);
        }
        while (stosHeight>0 && stosAkcji[stosHeight-1]->numToTest == 0){
            toUndu = grafBinSearch(graf, numGraf, stosAkcji[stosHeight-1]->changeNode);
            set[toUndu] = 0;
            stosHeight--;
            memFree(stosAkcji[stosHeight]);
        }
        printf("\r%ld", stosHeight);
                
                // printf("A");
    }
    printf("\n");
    set[0] = 0;
            
    return 0;
}

long cutGrafGrow(node** graf, long numGraf, int margin, node*** out){
    if(numGraf <= 1){
        return 0;
    }

    long maxSize = numGraf*(200+margin)/400;
    long minSize = numGraf - maxSize;
    if(minSize == 0){
        minSize++;
        maxSize--;
    }

    printf("%ld - %ld\n", minSize, maxSize);
    if(maxSize < minSize){
        return 0;
    }

    sortGrafData(graf, numGraf);

    char set[numGraf];
    for(long i=0;i<numGraf;i++){
        set[i] = 0;
    }
    // node* grafA[numGraf];
    // node* grafB[numGraf];

    long numGrafA = grow(graf, numGraf, minSize, maxSize, set);

    // printf("\n");
    long nodeIds[numGrafA];
    long iNodes = 0;
    for(long i=0;i<numGraf;i++){
        if(set[i] == 1){
            nodeIds[iNodes] = graf[i]->id;
            iNodes++;
            // printf("%ld ", graf[i]->id);
        }
    }
    // printf("\n");

    returnNewGraf(graf, numGraf, nodeIds, numGrafA, out);

    return numGrafA;
}