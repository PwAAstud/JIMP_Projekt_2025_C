#ifndef GRAF_H
#define GRAF_H

typedef struct nodeIn{
    long id;
    // obustonie
    struct nodeIn** conetion;
    // ilość połączeń
    long n;
} node;

typedef struct weightedNodeIn{
    long* ids;
    long nId;
    // obustonie
    struct weightedNodeIn** conetion;
    long* weighted;
    // ilość połączeń
    long nCon;
} weightedNode;

weightedNode** grafToWeighted(node** graf, long n);

void freeWeightedNode(weightedNode* node);

// zwraca index punktu w grafie
long minimumCutPhase(weightedNode** graf, long n);

void sortFunc(node** graf, long n);

void sortGrafData(node** graf, long n);

// zwraca gługoś nowego grafu
// *graf* to wpęłni połączony graf
// node*** out wstażnik gdzie zapisać
long cutGraf(node** graf, long n, int margin, node*** out);

#endif