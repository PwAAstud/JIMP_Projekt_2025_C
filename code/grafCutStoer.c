#include"..\header\grafCutStoer.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

long long current_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

static void printWeightedGraf(weightedNode** weightedGraf, long nWeightedGraf){
    for(long i=0; i<nWeightedGraf; i++){
        for(long j=0; j<weightedGraf[i]->nId; j++){
            printf("%ld ", weightedGraf[i]->ids[j]);
        }
        printf(",%ld: ",weightedGraf[i]->nCon);
        for(long j=0; j<weightedGraf[i]->nCon; j++){
            printf("(%ld, %ld) ", weightedGraf[i]->conetion[j]->ids[0], weightedGraf[i]->weighted[j]);
        }
        printf("\n");
    }
}

int sortujRosnaco( const void* a, const void* b){
    return *(long*)a - *(long*)b;
}

int sortujRosnacoPointers( const void* a, const void* b){
    void* pa = *(void**)a;
    void* pb = *(void**)b;
    // printf("%p,%p\n", pa, pb);
    // return pa-pb;
    if(pa > pb){return 1; }
    if(pa < pb){return -1; }
    return 0;
}

static long binSherchPointers(void** list, long numList, void* find){
    long left = -1;
    long right = numList;
    long center = (right + left)/2;

    while (right - left > 1){
        if(list[center] == find){
            return center;
        }
        if(list[center] < find){
            left = center;
        }else{
            right = center;
        }
        center = (right + left)/2;
    }
    return -1;
}

weightedNode** grafToWeighted(node** graf, long n){
    weightedNode** retVal = malloc(sizeof(weightedNode*)*n);
    if(!retVal){ fprintf(stderr, "[!] nie ma pamicei 1"); exit(1);}

    for(long i=0; i<n; i++){
        retVal[i] = malloc(sizeof(weightedNode));
        if(!retVal[i]){ fprintf(stderr, "[!] nie ma pamicei 2"); exit(2);}

        retVal[i]->nId = 1;
        retVal[i]->ids = malloc(sizeof(long)*retVal[i]->nId);
        if(!retVal[i]->ids){ fprintf(stderr, "[!] nie ma pamicei 3"); exit(3);}
        retVal[i]->ids[0] = graf[i]->id;
    }

    for(long i=0; i<n; i++){
        retVal[i]->nCon = graf[i]->n;
        retVal[i]->conetion = malloc(sizeof(weightedNode*)*graf[i]->n);
        if(!retVal[i]->conetion){ fprintf(stderr, "[!] nie ma pamicei"); exit(4);}
        retVal[i]->weighted = malloc(sizeof(long)*graf[i]->n);
        if(!retVal[i]->weighted){ fprintf(stderr, "[!] nie ma pamicei"); exit(5);}

        // zakladam ze zarówno graf jaki i conetion sa posotowanie rosnaco
        long findIndex = 0;
        for(long j=0; j<graf[i]->n; j++){
            findIndex = grafBinSearch(graf, n, graf[i]->conetion[j]->id);
            // for(; graf[i]->conetion[j]->id != graf[findIndex]->id; findIndex++);
            // findIndex = (graf[0] - graf[i]->conetion[j])/sizeof(node*);
            retVal[i]->conetion[j] = retVal[findIndex];
            retVal[i]->weighted[j] = 1;
        }
    }
    return retVal;
}

static void freeWeightedNode(weightedNode* node){
    free(node->conetion);
    free(node->ids);
    free(node->weighted);
    free(node);
}

// static void addToBlock(weightedNode** graf, long n, long* weighted, weightedNode* node){
//     // long findIndex = 0;
//     // for(;graf[findIndex] != node; findIndex++);
//     long findIndex = binSherchPointers((void**)graf, n, node);
//     weighted[findIndex] = -1;

//     for(long i=0; i<node->nCon; i++){
//         findIndex = binSherchPointers((void**)graf, n, node->conetion[i]);
//         // for(findIndex = 0;node->conetion[i] != graf[findIndex];findIndex++);
//         if(weighted[findIndex] == -1){
//             continue;
//         }
//         weighted[findIndex] += node->weighted[i];
//     }
//     // for(long j=0; j<n; j++){
//     //     printf("%3ld ", weighted[j]);
//     // }
//     // printf("\n");
// }

// toCheck i weighted musza byc gotowe do powiekszenia
// nodeToAdd->conetion musza byc posortowane
static long addToBlock(weightedNode** toCheck, long* weighted, long numToCheck, weightedNode** graf, char* visited, long numGraf, weightedNode* nodeToAdd){
    // long long start = current_time_ns();

    long remove = binSherchPointers((void**)toCheck, numToCheck, nodeToAdd);
    if(remove >= 0){
        for(long i=remove+1; i<numToCheck; i++){
            toCheck[i-1] = toCheck[i];
            weighted[i-1] = weighted[i];
        }
        numToCheck--;
    }

    // printf("%lld ", current_time_ns() - start);

    long indexInGraf;
    indexInGraf = binSherchPointers((void**)graf, numGraf, nodeToAdd);
    visited[indexInGraf] = 1;

    // start = current_time_ns();

    weightedNode* tempCon[numToCheck + nodeToAdd->nCon];
    long tempaWeig[numToCheck + nodeToAdd->nCon];
    long indexTemp = 0;
    long indexLeft = 0;
    long indexRight = 0;
    while(indexLeft < numToCheck && indexRight < nodeToAdd->nCon){
        if(toCheck[indexLeft] == nodeToAdd->conetion[indexRight]){
            tempCon[indexTemp] = toCheck[indexLeft];
            tempaWeig[indexTemp] = weighted[indexLeft] + nodeToAdd->weighted[indexRight];

            indexTemp++; indexLeft++; indexRight++;

        }else if(toCheck[indexLeft] < nodeToAdd->conetion[indexRight]){
            tempCon[indexTemp] = toCheck[indexLeft];
            tempaWeig[indexTemp] = weighted[indexLeft];

            indexTemp++; indexLeft++;

        }else{
            indexInGraf = binSherchPointers((void**)graf, numGraf, nodeToAdd->conetion[indexRight]);
            if(visited[indexInGraf] != 1){
                tempCon[indexTemp] = nodeToAdd->conetion[indexRight];
                tempaWeig[indexTemp] = nodeToAdd->weighted[indexRight];
                indexTemp++;
            }
            indexRight++;
        }
    }
    for(; indexLeft < numToCheck; indexLeft++){
        tempCon[indexTemp] = toCheck[indexLeft];
        tempaWeig[indexTemp] = weighted[indexLeft];
        indexTemp++;
    }
    for(; indexRight < nodeToAdd->nCon; indexRight++){
        indexInGraf = binSherchPointers((void**)graf, numGraf, nodeToAdd->conetion[indexRight]);
        if(visited[indexInGraf] == 1){
            continue;
        }
        tempCon[indexTemp] = nodeToAdd->conetion[indexRight];
        tempaWeig[indexTemp] = nodeToAdd->weighted[indexRight];

        indexTemp++;
    }
    // printf("%lld ", current_time_ns() - start);

    // start = current_time_ns();
    // // out
    for(long i=0; i < indexTemp; i++){
        toCheck[i] = tempCon[i];
        weighted[i] = tempaWeig[i];
    }
    // printf("%lld\n", current_time_ns() - start);

    // printf("\r__________________________________________________\r");
    return indexTemp;
}

// index punktu
static long findNextNode(weightedNode** toCheck, long* weighted, long numToCheck){
    long maxIndex = numToCheck-1;
    for(long i=numToCheck-2; i>=0; i--){
        if(weighted[maxIndex] < weighted[i]){
            maxIndex = i;
        }
        else if(weighted[maxIndex] == weighted[i] && toCheck[maxIndex]->nId > toCheck[i]->nId){
            maxIndex = i;
        }
    }
    return maxIndex;
}

static long minimumCutPhase(weightedNode** graf, long numGraf){
    
    // para list to przesztiwania
    weightedNode* toCheck[numGraf]; 
    long weighted[numGraf];
    long numToCheck = 0;
    
    // -1 jest w blocku
    char visited[numGraf];
    for(long i=0; i<numGraf; i++){
        visited[i] = 0;
    }
    // printf("%d\n", smalSise);
    numToCheck = addToBlock(toCheck, weighted, numToCheck, graf, visited, numGraf, graf[0]);
    long nextNode;
    for(long i=2; i<numGraf; i++){
        nextNode = findNextNode(toCheck, weighted, numToCheck);
        numToCheck = addToBlock(toCheck, weighted, numToCheck, graf, visited, numGraf, toCheck[nextNode]);
    }
    
    return binSherchPointers((void**)graf, numGraf, toCheck[0]);
}

// graf[nodeIndex]
// index to ktury punkt z conetion
static long sumaWeighted(weightedNode* node, long index){
    long retVal = 0;
    for(long i=0; i<node->conetion[index]->nCon; i++){
        retVal += node->conetion[index]->weighted[i];
    }
    return retVal - node->weighted[index];
}

// zwraca index z listy conection w wybranego punktu
static weightedNode* choseNodeToMerge(weightedNode* baseNode, long n, long max){
    long chosenNodeIndex = 0;
    long sumChosenNode = sumaWeighted(baseNode, 0);
    int isInRange = (baseNode->nId + baseNode->conetion[0]->nId) <= max;
    long newSum = 0;
    long newIsInRange = 0;
    for(long i=1; i<baseNode->nCon; i++){
        newIsInRange = (baseNode->nId + baseNode->conetion[i]->nId) <= max;
        newSum = sumaWeighted(baseNode, i);
        if(newIsInRange < isInRange){
            continue;
        }
        if(newIsInRange > isInRange){
            goto changeNodeToMerge;
        }
        // newIsInRange == isInRange
        if(baseNode->weighted[chosenNodeIndex] > baseNode->weighted[i]){
            continue;
        }
        if(baseNode->weighted[chosenNodeIndex] < baseNode->weighted[i]){
            goto changeNodeToMerge;
        }
        // baseNode->weighted[chosenNodeIndex] == baseNode->weighted[i]
        if(sumChosenNode > newSum){
            goto changeNodeToMerge;
        }

        continue;
        changeNodeToMerge:
        chosenNodeIndex = i;
        sumChosenNode = newSum;
        isInRange = newIsInRange;
    }
    return baseNode->conetion[chosenNodeIndex];
}

static void removeConection(weightedNode* node, long indexToRemodw){
    for(long z=indexToRemodw+1;z<node->nCon;z++){
        node->conetion[z-1] = node->conetion[z];
        node->weighted[z-1] = node->weighted[z];
    }
    node->nCon--;
}

// dopisuj polaczenie i wagi z listy nodeIn do nodeFrom w sposub posortowany (obie listy musza byc posortowane)
static void margeConec(weightedNode* nodeIn, weightedNode* nodeFrom){
    long newLen = nodeIn->nCon+nodeFrom->nCon;

    nodeIn->conetion = realloc(nodeIn->conetion, newLen* sizeof(weightedNode*));
    if(!nodeIn->conetion && 0!=newLen){ fprintf(stderr, "[!] nie ma pamicei 5"); exit(7);}
    nodeIn->weighted = realloc(nodeIn->weighted, newLen* sizeof(long));
    if(!nodeIn->weighted && 0!=newLen){ fprintf(stderr, "[!] nie ma pamicei 6"); exit(8);}
    weightedNode* tempCon[newLen];
    long tempaWeig[newLen];
    long indexTemp = 0;
    long indexIn = 0;
    long indexFrom = 0;
    for(; indexIn < nodeIn->nCon && indexFrom < nodeFrom->nCon; indexTemp++){
        if(nodeIn->conetion[indexIn] <= nodeFrom->conetion[indexFrom]){
            tempCon[indexTemp] = nodeIn->conetion[indexIn];
            tempaWeig[indexTemp] = nodeIn->weighted[indexIn];
            indexIn++;
        }else{
            tempCon[indexTemp] = nodeFrom->conetion[indexFrom];
            tempaWeig[indexTemp] = nodeFrom->weighted[indexFrom];
            indexFrom++;
        }
    }
    for(; indexIn < nodeIn->nCon; indexIn++){
        tempCon[indexTemp] = nodeIn->conetion[indexIn];
        tempaWeig[indexTemp] = nodeIn->weighted[indexIn];
        indexTemp++;
    }
    for(; indexFrom < nodeFrom->nCon; indexFrom++){
        tempCon[indexTemp] = nodeFrom->conetion[indexFrom];
        tempaWeig[indexTemp] = nodeFrom->weighted[indexFrom];
        indexTemp++;
    }
    // out
    for(long i=0; i < newLen; i++){
        nodeIn->conetion[i] = tempCon[i];
        nodeIn->weighted[i] = tempaWeig[i];
    }
    nodeIn->nCon = newLen;
}

static void changeConWithOrderSaved(weightedNode* in, weightedNode* from, weightedNode* to){
    long index = binSherchPointers((void**)in->conetion, in->nCon, from);
    in->conetion[index] = to;
    // down
    weightedNode* temp;
    for(;index>0 && in->conetion[index-1] > in->conetion[index]; index--){
        temp = in->conetion[index];
        in->conetion[index] = in->conetion[index-1];
        in->conetion[index-1] = temp;
    }
    //up
    index++;
    for(;index<in->nCon && in->conetion[index-1] > in->conetion[index]; index++){
        temp = in->conetion[index];
        in->conetion[index] = in->conetion[index-1];
        in->conetion[index-1] = temp;
    }
}

static void mergeWithGraf(weightedNode** graf, long n, long nodeIndex, long maxSize){
    long i;
    weightedNode* deletNode = graf[nodeIndex];
    if(deletNode->nCon == 0){
        fprintf(stderr, "[!] node nie ma połączeń\n"); exit(5);
    }

    weightedNode* mergeWith = choseNodeToMerge(deletNode, n, maxSize);

    // printf("%p %p\n", mergeWith, deletNode);
    // pozbycie sie polaczenie miedzy mergeWith i deletNode
    i = binSherchPointers((void**)deletNode->conetion, deletNode->nCon, mergeWith);
    // for(i=0; deletNode->conetion[i] != mergeWith; i++);
    removeConection(deletNode, i);
    // printf(">%p, %d\n",deletNode, mergeWith->nCon);
    i = binSherchPointers((void**)mergeWith->conetion, mergeWith->nCon, deletNode);
    // for(i=0; mergeWith->conetion[i] != deletNode; i++);
    removeConection(mergeWith, i);

    // laczenie id
    mergeWith->ids = realloc(mergeWith->ids, (mergeWith->nId + deletNode->nId)*sizeof(long));
    if(!mergeWith->ids){ fprintf(stderr, "[!] nie ma pamicei 4"); exit(6);}
    for(i = 0; i<deletNode->nId; i++){
        // printf("%d ",deletNode->ids[i]);
        mergeWith->ids[mergeWith->nId+i] = deletNode->ids[i];
    }
    mergeWith->nId += deletNode->nId;

    for(i=0; i<deletNode->nCon; i++){
        long z;
        long j = binSherchPointers((void**)mergeWith->conetion, mergeWith->nCon, deletNode->conetion[i]);
        if(j>=0){
            // sumowanie wag przy tych samych
            mergeWith->weighted[j] += deletNode->weighted[i];

            // pozbycie polaczenia do deletnode z punktu wspulnie po jego stnie
            z = binSherchPointers((void**)mergeWith->conetion[j]->conetion, mergeWith->conetion[j]->nCon, deletNode);
            // for(z=0; mergeWith->conetion[j]->conetion[z] != deletNode; z++);
            removeConection(mergeWith->conetion[j], z);

            // zaktualizowanie sily polacznie po drugiej stronie
            z = binSherchPointers((void**)mergeWith->conetion[j]->conetion, mergeWith->conetion[j]->nCon, mergeWith);
            // for(z=0; mergeWith->conetion[j]->conetion[z] != mergeWith; z++);
            mergeWith->conetion[j]->weighted[z] = mergeWith->weighted[j];

            // pozbycie sie powturzenia
            removeConection(deletNode, i);
            i--;
        }else{
            changeConWithOrderSaved(deletNode->conetion[i], deletNode, mergeWith);
        }

        // long j;
        // long z;
        // isDuplicat = 0;
        // sumowanie wag przy tych samych
        // for(j=0; j<mergeWith->nCon; j++){
        //     if(mergeWith->conetion[j] != deletNode->conetion[i]){
        //         continue;
        //     }
        //     isDuplicat = 1;
        //     mergeWith->weighted[j] += deletNode->weighted[i];

        //     // pozbycie polaczenia do deletnode z punktu wspulnie po jego stnie
        //     for(z=0; mergeWith->conetion[j]->conetion[z] != deletNode; z++);
        //     removeConection(mergeWith->conetion[j], z);

        //     // zaktualizowanie sily polacznie po drugiej stronie
        //     for(z=0; mergeWith->conetion[j]->conetion[z] != mergeWith; z++);
        //     mergeWith->conetion[j]->weighted[z] = mergeWith->weighted[j];

        //     // pozbycie sie powturzenia
        //     removeConection(deletNode, i);
        //     i--;

        //     break;
        // }

        // if(!isDuplicat){ // nieduplikat
        //     for(z=0; deletNode->conetion[i]->conetion[z] != deletNode; z++);
        //     deletNode->conetion[i]->conetion[z] = mergeWith;
        //     // printf(" %d ", z>= deletNode->conetion[i]->nCon);
        // }
    }

    margeConec(mergeWith, deletNode);

    // mergeWith->conetion = realloc(mergeWith->conetion, (mergeWith->nCon+deletNode->nCon)* sizeof(weightedNode*));
    // if(!mergeWith->conetion && 0!=(mergeWith->nCon+deletNode->nCon)){ fprintf(stderr, "[!] nie ma pamicei 5"); exit(7);}
    // mergeWith->weighted = realloc(mergeWith->weighted, (mergeWith->nCon+deletNode->nCon)* sizeof(long));
    // if(!mergeWith->weighted && 0!=(mergeWith->nCon+deletNode->nCon)){ fprintf(stderr, "[!] nie ma pamicei 6"); exit(8);}
    // for(i=0; i<deletNode->nCon; i++){
    //     mergeWith->conetion[mergeWith->nCon+i] = deletNode->conetion[i];
    //     mergeWith->weighted[mergeWith->nCon+i] = deletNode->weighted[i];
    // }
    // mergeWith->nCon+=deletNode->nCon;
    // printf("%ld \n", deletNode->nCon);
    
    freeWeightedNode(deletNode);
    for(i=nodeIndex+1; i<n; i++){
        graf[i-1] = graf[i];
    }

}

// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
long cutGrafStoner(node** graf, long n, int margin, node*** out){
    if(n <= 1){
        return 0;
    }
    
    long maxSize = (n*(200+margin))/400;
    long minSize = n - maxSize;
    if(maxSize >= n){
        maxSize = n-1;
        minSize = 1;
    }
    // printf("%ld - %ld\n", minSize, maxSize);
    if(maxSize < minSize){
        return 0;
    }
    sortGrafData(graf, n);
    // printf("%d\n", isGrafConected(graf, n));
    // printGraf(graf, n);
    // for(long i=0;i<n;i++){
    //     printf("%ld:%ld ", graf[i]->id,graf[i]->n);
    // }
    // printf("\n");
    
    weightedNode** weightedGraf = grafToWeighted(graf, n);
    long nWeightedGraf = n;
    // printWeightedGraf(weightedGraf, nWeightedGraf);
    // printf("\n");

    qsort( weightedGraf, nWeightedGraf, sizeof(weightedNode*), sortujRosnacoPointers);
    for(long i=0; i<nWeightedGraf; i++){
        qsort( weightedGraf[i]->conetion, weightedGraf[i]->nCon, sizeof(weightedNode*), sortujRosnacoPointers);
        // for(long j=0; j<weightedGraf[i]->nCon; j++){
        //     printf("%p\n", weightedGraf[i]->conetion[j]);
        // }
        // printf("\n");
    }

    struct{
        long nodeIds[n];
        long n;
        long cut;
    } bestCut;
    bestCut.n = 0;
    bestCut.cut = 0x7fffffff; // licze ze nie bedzie tak dzurzego ciencia. potrebuje liczby inf na start
    
    long newCut;

    while (nWeightedGraf > 1){
        //printf("\r%ld", nWeightedGraf);

        // clock_t start = clock();
        long farderNode = minimumCutPhase(weightedGraf, nWeightedGraf);
        // printf("chosen: %ld\n", weightedGraf[farderNode]->ids[0]);
        // printf("size: %ld\n", weightedGraf[farderNode]->nId);

        // printf("%f\n", (float)(clock() - start) / CLOCKS_PER_SEC);

        if(weightedGraf[farderNode]->nId >= minSize && weightedGraf[farderNode]->nId <= maxSize){
            newCut = 0;
            for(long i=0; i< weightedGraf[farderNode]->nCon; i++){
                newCut += weightedGraf[farderNode]->weighted[i];
            }
            if(newCut < bestCut.cut){
                for(long i=0; i< weightedGraf[farderNode]->nId; i++){
                    bestCut.nodeIds[i] = weightedGraf[farderNode]->ids[i];
                }
                bestCut.cut = newCut;
                bestCut.n = weightedGraf[farderNode]->nId;
            }
        }
        // printf("%ld\n", farderNode);
        mergeWithGraf(weightedGraf, nWeightedGraf, farderNode, maxSize);
        nWeightedGraf--;
        // printf("\n");
        // printWeightedGraf(weightedGraf, nWeightedGraf);
    }
    qsort( bestCut.nodeIds, bestCut.n, sizeof(long), sortujRosnaco);
    
    returnNewGraf(graf, n, bestCut.nodeIds, bestCut.n, out);
    
    for(long i=0; i<nWeightedGraf; i++){
        freeWeightedNode(weightedGraf[i]);
    }
    
    printf("\n");
    return bestCut.n;
}
