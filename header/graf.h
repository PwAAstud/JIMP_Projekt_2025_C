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

// nie usuwa polaczen miedzy outA i outB
// set to lista 0 i 1 symbolizujaca przynaleznosc do a lub b
void makeSubGrafs(node** graf, char * set, long numGrafSet, node** outA, node** outB);

// wymaga posortowaego grafu funkcja *sortGrafData*
int isGrafConected(node** graf, long numGraf);

void returnNewGraf(node** graf, long num_graf, long* newGrafIds, long num_new, node*** out);

// wymaga posotowanego grafu
// zwraca -1 jak nie znajdzie
long grafBinSearch(node** graf, long numGraf, long findId);

#endif