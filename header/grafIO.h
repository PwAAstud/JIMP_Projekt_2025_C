#ifndef GRAFIO_H
#define GRAFIO_H
#include"graf.h"

typedef struct{
    long line1;
    long* line2;
    long numLine2;
    long* line3;
    long numLine3;
    node** graf;
    long numGraf;
} grafData;

void printGrafData(grafData* data);

grafData* loadGrafData(char* inName);

void saveGrafDataTxt(char* outName, grafData* data, long udaneCieca);

void saveGrafDataBin(char* outName, grafData* data, long udaneCieca);

#endif