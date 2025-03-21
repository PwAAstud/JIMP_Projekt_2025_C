#include"..\header\graf.h"
#include<stdio.h>
#include<stdlib.h>

void printWeightedGraf(weightedNode** weightedGraf, long nWeightedGraf){
    for(long i=0; i<nWeightedGraf; i++){
        for(long j=0; j<weightedGraf[i]->nId; j++){
            printf("%ld ", weightedGraf[i]->ids[j]);
        }
        printf(": ");
        for(long j=0; j<weightedGraf[i]->nCon; j++){
            printf("(%ld, %ld) ", weightedGraf[i]->conetion[j]->ids[0], weightedGraf[i]->weighted[j]);
        }
        printf("\n");
    }
}

int sortujRosnaco( const void* a, const void* b){
    return *(long*)a - *(long*)b;
}

weightedNode** grafToWeighted(node** graf, long n){
    weightedNode** retVal = malloc(sizeof(weightedNode*)*n);
    if(!retVal){ fprintf(stderr, "[!] nie ma pamicei");}

    for(long i=0; i<n; i++){
        retVal[i] = malloc(sizeof(weightedNode));
        if(!retVal[i]){ fprintf(stderr, "[!] nie ma pamicei");}

        retVal[i]->nId = 1;
        retVal[i]->ids = malloc(sizeof(long)*retVal[i]->nId);
        if(!retVal[i]->ids){ fprintf(stderr, "[!] nie ma pamicei");}
        retVal[i]->ids[0] = graf[i]->id;
    }

    for(long i=0; i<n; i++){
        retVal[i]->nCon = graf[i]->n;
        retVal[i]->conetion = malloc(sizeof(weightedNode*)*graf[i]->n);
        if(!retVal[i]->conetion){ fprintf(stderr, "[!] nie ma pamicei");}
        retVal[i]->weighted = malloc(sizeof(long)*graf[i]->n);
        if(!retVal[i]->weighted){ fprintf(stderr, "[!] nie ma pamicei");}

        // zakladam ze zarówno graf jaki i conetion sa posotowanie rosnaco
        long findIndex = 0;
        for(long j=0; j<graf[i]->n; j++){
            for(; graf[i]->conetion[j]->id != graf[findIndex]->id; findIndex++);
            // findIndex = (graf[0] - graf[i]->conetion[j])/sizeof(node*);
            retVal[i]->conetion[j] = retVal[findIndex];
            retVal[i]->weighted[j] = 1;
        }
    }
    return retVal;
}

void freeWeightedNode(weightedNode* node){
    free(node->conetion);
    free(node->ids);
    free(node->weighted);
    free(node);
}

void addToBlock(weightedNode** graf, long n, long* weighted, weightedNode* node){
    long findIndex = 0;
    for(;graf[findIndex] != node; findIndex++);
    weighted[findIndex] = -1;

    for(long i=0; i<node->nCon; i++){
        for(findIndex = 0;node->conetion[i] != graf[findIndex];findIndex++);
        if(weighted[findIndex] == -1){
            continue;
        }
        weighted[findIndex] += node->weighted[i];
    }
    // for(long j=0; j<n; j++){
    //     printf("%3ld ", weighted[j]);
    // }
    // printf("\n");
}

// index punktu
long findNextNode(weightedNode** graf, long n, long* weighted){
    long maxIndex = 0;
    for(long i=1; i<n; i++){
        if(weighted[maxIndex] < weighted[i]){
            maxIndex = i;
        }
        else if(weighted[maxIndex] == weighted[i] && graf[maxIndex]->nId > graf[i]->nId){
            maxIndex = i;
        }
    }
    return maxIndex;
}

long minimumCutPhase(weightedNode** graf, long n){
    // -1 jest w blocku
    long weighted[n];
    for(long i=0; i<n; i++){
        weighted[i] = 0;
    }

    addToBlock(graf, n, weighted, graf[0]);
    long nextNode;
    for(long i=2; i<n; i++){
        nextNode = findNextNode(graf, n, weighted);
        addToBlock(graf, n, weighted, graf[nextNode]);
    }

    long retVal;
    for(retVal = 0; weighted[retVal] == -1; retVal++);
    return retVal;
}

// graf[nodeIndex]
// index to ktury punkt z conetion
long sumaWeighted(weightedNode* node, long index){
    long retVal = 0;
    for(long i=0; i<node->conetion[index]->nCon; i++){
        retVal += node->conetion[index]->weighted[i];
    }
    return retVal - node->weighted[index];
}

// zwraca index z listy conection w wybranego punktu
weightedNode* choseNodeToMerge(weightedNode* baseNode, long n){
    long chosenNodeIndex = 0;
    long sumChosenNode = sumaWeighted(baseNode, 0);
    long newSum = 0;
    for(long i=1; i<baseNode->nCon; i++){
        if(baseNode->weighted[chosenNodeIndex] > baseNode->weighted[i]){
            continue;
        }
        newSum = sumaWeighted(baseNode, i);
        // printf("%ld - %ld\n", graf[nodeIndex]->weighted[i], newSum);
        // printf("%ld\n", newSum);
        if(baseNode->weighted[chosenNodeIndex] < baseNode->weighted[i]){
            chosenNodeIndex = i;
            sumChosenNode = newSum;
            continue;
        }
        if(sumChosenNode > newSum){
            chosenNodeIndex = i;
            sumChosenNode = newSum;
            continue;
        }
    }
    return baseNode->conetion[chosenNodeIndex];
}

void removeConection(weightedNode* node, long indexToRemodw){
    for(long z=indexToRemodw+1;z<node->nCon;z++){
        node->conetion[z-1] = node->conetion[z];
        node->weighted[z-1] = node->weighted[z];
    }
    node->nCon--;
}

void mergeWithGraf(weightedNode** graf, long n, long nodeIndex){
    long i;
    weightedNode* deletNode = graf[nodeIndex];

    if(deletNode->nCon == 0){
        fprintf(stderr, "[!] node nie ma połączeń\n");
    }
    weightedNode* mergeWith = choseNodeToMerge(deletNode, n);

    //tymczasiwe do testow
    // deletNode = graf[1];
    // mergeWith = graf[0];
    // nodeIndex = 1;
    // test end

    // pozbycie sie polaczenie miedzy mergeWith i deletNode
    for(i=0; mergeWith->conetion[i] != deletNode; i++);
    removeConection(mergeWith, i);

    for(i=0; deletNode->conetion[i] != mergeWith; i++);
    removeConection(deletNode, i);

    // laczenie id
    mergeWith->ids = realloc(mergeWith->ids, (mergeWith->nId+deletNode->nId)* sizeof(long));
    if(!mergeWith->ids){ fprintf(stderr, "[!] nie ma pamicei");}
    for(i = 0; i<deletNode->nId; i++){
        mergeWith->ids[mergeWith->nId+i] = deletNode->ids[i];
    }
    mergeWith->nId += deletNode->nId;

    for(i=0; i<deletNode->nCon; i++){
        long j;
        long z;
        // sumowanie wag przy tych samych
        for(j=0; j<mergeWith->nCon; j++){
            if(mergeWith->conetion[j] != deletNode->conetion[i]){
                continue;
            }
            mergeWith->weighted[j] += deletNode->weighted[i];

            // pozbycie polaczenia do deletnode z punktu wspulnie po jego stnie
            for(z=0; mergeWith->conetion[j]->conetion[z] != deletNode; z++);
            removeConection(mergeWith->conetion[j], z);

            // zaktualizowanie sily polacznie po drugiej stronie
            for(z=0; mergeWith->conetion[j]->conetion[z] != mergeWith; z++);
            mergeWith->conetion[j]->weighted[z] = mergeWith->weighted[j];

            // pozbycie sie powturzenia
            removeConection(deletNode, i);
            i--;

            break;
        }

        if(j == mergeWith->nCon){ // nieduplikat
            for(z=0; deletNode->conetion[i]->conetion[z] != deletNode; z++);
            deletNode->conetion[i]->conetion[z] = mergeWith;
        }
    }

    mergeWith->conetion = realloc(mergeWith->conetion, (mergeWith->nCon+deletNode->nCon)* sizeof(weightedNode*));
    if(!mergeWith->conetion){ fprintf(stderr, "[!] nie ma pamicei");}
    mergeWith->weighted = realloc(mergeWith->weighted, (mergeWith->nCon+deletNode->nCon)* sizeof(long));
    if(!mergeWith->weighted){ fprintf(stderr, "[!] nie ma pamicei");}

    for(i=mergeWith->nCon; i<mergeWith->nCon+deletNode->nCon; i++){
        mergeWith->conetion[i] = deletNode->conetion[i];
        mergeWith->weighted[i] = deletNode->weighted[i];
    }
    // printf("%ld \n", deletNode->nCon);
    mergeWith->nCon+=deletNode->nCon;


    freeWeightedNode(deletNode);
    for(i=nodeIndex+1; i<n; i++){
        graf[i-1] = graf[i];
    }
}

void sortFunc(node** graf, long n){
    // urzywamy shell sorta
    node* temp;
    for(long step = n/2; step > 0; step/=2){
        for(long i=step; i<n; i++){
            for(long j=i; j>=step; j-=step){
                if(graf[j]->id >= graf[j-step]->id){
                    break;
                }
                temp = graf[j];
                graf[j] = graf[j-step];
                graf[j-step] = temp;
            }
        }
    }
}

void sortGrafData(node** graf, long n){
    sortFunc(graf, n);
    for(long i=0; i<n; i++){
        sortFunc(graf[i]->conetion, graf[i]->n);
    }
}

// wymaga graf był posortowane po id
void removdEmptyConection(node** graf, long num_graf){
    long remowdConection, checkingId;
    for(long i=0; i<num_graf; i++){
        remowdConection = 0;
        checkingId = 0;
        // printf("%ld < ", graf[i]->id);
        for(long j=0; j<graf[i]->n; j++){
            graf[i]->conetion[j-remowdConection] = graf[i]->conetion[j];
            // printf("%ld %ld ", graf[i]->conetion[j]->id, graf[checkingId]->id);
            for(; checkingId<num_graf-1 && graf[i]->conetion[j]->id > graf[checkingId]->id; checkingId++){}
            // printf("%ld %ld %ld; ", graf[i]->conetion[j]->id, graf[checkingId]->id, checkingId);
            if(graf[i]->conetion[j]->id != graf[checkingId]->id){
                remowdConection++;
            }
        }
        // printf("\n");
        graf[i]->n -= remowdConection;
    }
    // printf("\n");
}

// wymaga graf i newGrafIds zeby były posortowane po id
void returnNewGraf(node** graf, long num_graf, long* newGrafIds, long num_new, node*** out){
    *(out) = malloc(sizeof(node)*num_new);
    if(!*(out)){ fprintf(stderr, "[!] nie ma pamicei");}

    // podzielenie na dwie czeski
    long fillOut = 0;
    for(long i=0; i<num_graf; i++){
        graf[i-fillOut] = graf[i];
        if(graf[i]->id == newGrafIds[fillOut]){
            (*out)[fillOut] = graf[i];
            fillOut++;
        }
    }
    num_graf -= num_new;

    removdEmptyConection(graf, num_graf);
    removdEmptyConection(*out, num_new);

}

// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
long cutGraf(node** graf, long n, int margin, node*** out){
    if(n <= 1){
        return 0;
    }

    long maxSize = n*(200+margin)/400;
    long minSize = n - maxSize;
    // printf("%ld - %ld\n", maxSize, minSize);
    if(maxSize < minSize){
        return 0;
    }

    sortGrafData(graf, n);

    weightedNode** weightedGraf = grafToWeighted(graf, n);
    long nWeightedGraf = n;

    // for(long i=0; i<n; i++){
    //     printf("%ld: ", graf[i]->id);
    //     for(long j=0; j<graf[i]->n; j++){
    //         printf("%ld ", graf[i]->conetion[j]->id);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // printWeightedGraf(weightedGraf, nWeightedGraf);

    struct{
        long nodeIds[n];
        long n;
        long cut;
    } bestCut;
    bestCut.n = 0;
    bestCut.cut = 0x7fffffff; // licze ze nie bedzie tak dzurzego ciencia. potrebuje liczby inf na start
    
    long newCut;
    while (nWeightedGraf > 2){
        long farderNode = minimumCutPhase(weightedGraf, nWeightedGraf);
        // printf("chosen: %ld\n", weightedGraf[farderNode]->ids[0]);
        
        // printf("size: %ld\n", weightedGraf[farderNode]->nId);
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
        mergeWithGraf(weightedGraf, nWeightedGraf, farderNode);
        nWeightedGraf--;
    
        // printf("\n");
        // printWeightedGraf(weightedGraf, nWeightedGraf);
    }
    // printf("\n");

    qsort( bestCut.nodeIds, bestCut.n, sizeof(long), sortujRosnaco);
    
    // printf("%ld: ", bestCut.cut);
    // for(long i=0; i< bestCut.n; i++){
    //     printf("%ld ", bestCut.nodeIds[i]);
    // }
    // printf("\n\n");

    // bestCut.nodeIds = {0,1,2};

    returnNewGraf(graf, n, bestCut.nodeIds, bestCut.n, out);

    for(long i=0; i<nWeightedGraf; i++){
        freeWeightedNode(weightedGraf[i]);
    }

    return bestCut.n;
}
