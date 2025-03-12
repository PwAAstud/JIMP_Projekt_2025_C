#ifndef GRAF_H
#define GRAF_H

typedef struct nodeIn{
    long id;
    // obustonie
    struct nodeIn** conetion;
} node;

// zwraca gługoś nowego grafu
// *graf* to wpęłni połączony graf
long cut(node** graf, long n, int margin, node** out);

#endif