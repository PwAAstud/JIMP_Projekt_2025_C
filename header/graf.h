#ifndef GRAF_H
#define GRAF_H

typedef struct nodeIn{
    long id;
    // obustonie
    struct nodeIn** conetion;
    // ilość połączeń
    long n;
} node;

void printGraf(node** graf, long n);

void sortFunc(node** graf, long n);

void sortGrafData(node** graf, long n);

void removdEmptyConection(node** graf, long num_graf);

void returnNewGraf(node** graf, long num_graf, long* newGrafIds, long num_new, node*** out);

// wymaga posotowanego grafu
// zwraca -1 jak nie znajdzie
long grafBinSearch(node** graf, long numGraf, long findId);

#endif