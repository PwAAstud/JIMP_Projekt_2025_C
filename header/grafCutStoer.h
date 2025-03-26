#ifndef GRAFCUTSTOER_H
#define GRAFCUTSTOER_H
#include"graf.h"

typedef struct weightedNodeIn{
    long* ids;
    long nId;
    // obustonie
    struct weightedNodeIn** conetion;
    long* weighted;
    // ilość połączeń
    long nCon;
} weightedNode;

// zwraca gługoś nowego grafu
// *graf* to wpęłni połączony graf
// node*** out wstażnik gdzie zapisać
long cutGrafStoner(node** graf, long n, int margin, node*** out);

#endif