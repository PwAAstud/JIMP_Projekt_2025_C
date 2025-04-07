#ifndef PRINT_H
#define PRINT_H
#include"graf.h"

typedef struct zapiszmito{
    char* linia1;
    char* linia2;
    char* linia3;
    node** nodes;
    long rozmiar;
} wczytanie;
wczytanie* scan(char* in);
void prit(wczytanie*, int, int);

#endif
