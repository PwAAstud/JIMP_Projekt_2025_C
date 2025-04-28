#include"../header/graf.h"
#include<stdio.h>
#include<stdlib.h>

void printGraf(node** graf, long n){
    for(long i=0; i<n; i++){
        printf("%ld: ", graf[i]->id);
        for(long j=0; j<graf[i]->n; j++){
            printf("%ld ", graf[i]->conetion[j]->id);
        }
        printf("\n");
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

// wymaga graf był posortowane po id
void removdEmptyConection(node** graf, long num_graf){
    // printGraf(graf, num_graf);
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
    // printGraf(graf, num_graf);
    // printf("---\n");
}

int isGrafConected(node** graf, long numGraf){
    // 0 niewadomo
    // 1 do odwiedzenia
    // -1 bylem
    char nodeVisited[numGraf];
    for(int i = 0; i < numGraf; i++){
        nodeVisited[i] = 0;
    }

    struct{
        long next[numGraf];
        long n;
    } stack;
    stack.n = 1;
    stack.next[0] = 0;
    

    int nodeToCheck;
    while (stack.n > 0){
        stack.n--;
        nodeToCheck = stack.next[stack.n];

        int j=0;
        for(long i = 0; i < graf[nodeToCheck]->n; i++){
            for(j=0; j<numGraf && graf[j]->id != graf[nodeToCheck]->conetion[i]->id; j++);
            if(j >= numGraf){
                continue;
            }
            if(nodeVisited[j] != 0){
                continue;
            }
            stack.next[stack.n] = j;
            stack.n++;
            nodeVisited[j] = 1;
        }
        nodeVisited[nodeToCheck] = -1;
    }
    
    // for(int z=0; z<numGraf; z++){
    //     printf("%d ", nodeVisited[z]);
    // }
    // printf("\n");
    for(nodeToCheck = 0; nodeToCheck < numGraf && nodeVisited[nodeToCheck] != 0; nodeToCheck++);
    return nodeToCheck == numGraf;
}

// wymaga graf i newGrafIds zeby były posortowane po id
void returnNewGraf(node** graf, long num_graf, long* newGrafIds, long num_new, node*** out){
    if(num_new == 0){
        return;
    }
    *out = malloc(sizeof(node*)*num_new);
    if(!(*out)){ fprintf(stderr, "[!] nie ma pamicei");}

    // podzielenie na dwie czeski
    long fillOut = 0;
    for(long i=0; i<num_graf; i++){
        graf[i-fillOut] = graf[i];
        if(fillOut<num_new && graf[i]->id == newGrafIds[fillOut]){
            // printf("%ld ", graf[i]->id);
            (*out)[fillOut] = graf[i];
            fillOut++;
        }
    }
    num_graf -= num_new;
    removdEmptyConection(graf, num_graf);
    removdEmptyConection(*out, num_new);
}

long grafBinSearch(node** graf, long numGraf, long findId){
    long l = -1;
    long r = numGraf;
    long m = (r+l)/2;

    while (r - l > 1){
        // printf("(%ld - %ld); ", l,r);
        if(graf[m]->id == findId){
            return m;
        }
        if(graf[m]->id < findId){
            l = m;
        }else{
            r = m;
        }

        m = (r+l)/2;
    }
    
    return -1;
}
