#ifndef GRAF_H
#define GRAF_H

typedef struct nodeIn{
    long id;
    // obustonie
    struct nodeIn** conetion;
    // ilość połączeń
    long n;
} node;

// zwraca gługoś nowego grafu
// *graf* to wpęłni połączony graf
long cutGraf(node** graf, long n, int margin, node** out);

#endif