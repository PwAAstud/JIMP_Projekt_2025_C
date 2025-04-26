#include"..\header\grafIO.h"
#include"..\header\graf.h"
#include"stdio.h"
#include"stdlib.h"

// co ile zwiekaszac pamiec
#define MEMORYJUMP 16

void printGrafData(grafData* data){
    for(int i=0; i<data->numLine2; i++){
        printf("%ld ", data->line2[i]);
    }
    printf("\n");
    for(int i=0; i<data->numLine3; i++){
        printf("%ld ", data->line3[i]);
    }
    printf("\n");
    printGraf(data->graf, data->numGraf);
};

// konczy sie na \n przecinki to ;
static long* readNumberLine(FILE* in, long* outLen){
    long* retVal = malloc(MEMORYJUMP*sizeof(long));
    if(!retVal){ fprintf(stderr, "[!] za malo pamieci"); return NULL;}

    // obecny index
    long readNumber = 0;
    // kiedy zwiekszyc pamiec
    long nextJump = 0;
    char c;
    do{
        if(nextJump==MEMORYJUMP){
            retVal = realloc(retVal, (readNumber+MEMORYJUMP)*sizeof(long));
            if(!retVal){ fprintf(stderr, "[!] za malo pamieci"); return NULL;}
            nextJump = 0;
        }
        fscanf(in, "%ld", &retVal[readNumber]);
        readNumber++;
        nextJump++;
        c = fgetc(in);
    } while (c != '\n' && c != EOF);

    *outLen = readNumber;
    return retVal;
}

static void makeConection(node** graf, long a, long b){
    if(graf[a]->n%MEMORYJUMP == 0){
        graf[a]->conetion = realloc(graf[a]->conetion, sizeof(node*)*(graf[a]->n+MEMORYJUMP));
        if(!graf[a]->conetion){ fprintf(stderr, "[!] za malo pamieci"); return;}
    }
    if(graf[b]->n%MEMORYJUMP == 0){
        graf[b]->conetion = realloc(graf[b]->conetion, sizeof(node*)*(graf[b]->n+MEMORYJUMP));
        if(!graf[b]->conetion){ fprintf(stderr, "[!] za malo pamieci"); return;}
    }
    
    graf[a]->conetion[graf[a]->n] = graf[b];
    graf[b]->conetion[graf[b]->n] = graf[a];
    
    graf[a]->n++;
    graf[b]->n++;
}

grafData* loadGrafData(char* inName){
    FILE* in = fopen(inName, "r");
    if(!in){ fprintf(stderr, "[!] zly plik podany"); return NULL;}

    grafData* retVal = malloc(sizeof(grafData));
    if(!retVal){ fprintf(stderr, "[!] za malo pamieci"); return NULL;}

    fscanf(in, "%ld", &retVal->line1);
    retVal->line2 = readNumberLine(in, &retVal->numLine2);
    retVal->line3 = readNumberLine(in, &retVal->numLine3);

    long numPolaczen;
    long* listaPolaczen = readNumberLine(in, &numPolaczen);
    
    long numPrzedzialow;
    long* listaPrzedzialow = readNumberLine(in, &numPrzedzialow);
    // inicjalizaca grafu
    retVal->graf = malloc(sizeof(node*)*retVal->numLine2);
    retVal->numGraf = retVal->numLine2;
    if(!retVal->graf){ fprintf(stderr, "[!] za malo pamieci"); return NULL;}
    for(long i=0; i<retVal->numLine2; i++){
        retVal->graf[i] = malloc(sizeof(node));
        if(!retVal->graf[i]){ fprintf(stderr, "[!] za malo pamieci"); return NULL;}
        retVal->graf[i]->id = i;
        retVal->graf[i]->n = 0;
        retVal->graf[i]->conetion = NULL;
    }

    // inicjalizaca poloaczen
    for(long i=1; i<numPrzedzialow; i++){
        long start = listaPrzedzialow[i-1];
        long end = listaPrzedzialow[i];
        // printf("%ld %ld:", start,end);
        for(long j=start+1; j<end; j++){
            // printf("%ld - %ld\n", listaPolaczen[start], listaPolaczen[j]);
            makeConection(retVal->graf, listaPolaczen[start], listaPolaczen[j]);
        }
    }
    long start = listaPrzedzialow[numPrzedzialow-1];
    long end = numPolaczen;
    for(long j=start+1; j<end; j++){
        // printf("%ld - %ld\n", listaPolaczen[start], listaPolaczen[j]);
        makeConection(retVal->graf, listaPolaczen[start], listaPolaczen[j]);
    }

    return retVal;
}

void saveGrafDataTxt(char* outName, grafData* data, long udaneCieca){
    FILE* out = fopen(outName, "w");
    
    fprintf(out, "%ld\n", udaneCieca);
    fprintf(out, "%ld\n", data->line1);

    fprintf(out, "%ld", data->line2[0]);
    for(long i=1; i<data->numLine2;i++){
        fprintf(out, ";%ld", data->line2[i]);
    }
    fprintf(out,"\n");

    fprintf(out, "%ld", data->line3[0]);
    for(long i=1; i<data->numLine3;i++){
        fprintf(out, ";%ld", data->line3[i]);
    }
    fprintf(out,"\n");

    sortGrafData(data->graf, data->numGraf);
    long przedzialy[data->numGraf+1];
    przedzialy[0] = 0;
    long numPrzedzialy = 1;
    long index=0;
    for(long i=0; i<data->numGraf; i++){
        long bigCon;
        for(bigCon=0; bigCon < data->graf[i]->n && data->graf[i]->conetion[bigCon]->id < data->graf[i]->id; bigCon++);
        if(bigCon == data->graf[i]->n){
            continue;
        }

        fprintf(out, "%ld;", data->graf[i]->id);
        index++;
        for(long j=bigCon; j<data->graf[i]->n;j++){
            fprintf(out, "%ld;", data->graf[i]->conetion[j]->id);
            index++;
        }
        przedzialy[numPrzedzialy] = index;
        numPrzedzialy++;
    }
    fseek(out, -1 ,SEEK_CUR);
    fprintf(out, "\n");

    fprintf(out, "0");
    for(long i=1; i<numPrzedzialy-1; i++){
        fprintf(out, ";%ld", przedzialy[i]);
    }
}

void saveGrafDataBin(char* outName, grafData* data, long udaneCieca){
    FILE* out = fopen(outName, "wb");
    long endofLine = -2;

    fwrite( &udaneCieca, 4, 1, out);
    fwrite( &endofLine, 4,1, out);
    fwrite( &data->line1, 4, 1, out);
    fwrite( &endofLine, 4,1, out);

    fwrite( data->line2, 4, data->numLine2, out);
    fwrite( &endofLine, 4,1, out);

    fwrite( data->line3, 4, data->numLine3, out);
    fwrite( &endofLine, 4,1, out);

    sortGrafData(data->graf, data->numGraf);
    long przedzialy[data->numGraf+1];
    przedzialy[0] = 0;
    long numPrzedzialy = 1;
    long index=0;
    for(long i=0; i<data->numGraf; i++){
        long bigCon;
        for(bigCon=0; bigCon < data->graf[i]->n && data->graf[i]->conetion[bigCon]->id < data->graf[i]->id; bigCon++);
        if(bigCon == data->graf[i]->n){
            continue;
        }

        fwrite( &data->graf[i]->id, 4,1, out);
        index++;
        for(long j=bigCon; j<data->graf[i]->n;j++){
            fwrite( &data->graf[i]->conetion[j]->id, 4,1, out);
            index++;
        }
        przedzialy[numPrzedzialy] = index;
        numPrzedzialy++;
    }

    fwrite(przedzialy, 4, numPrzedzialy-1, out);

}

#undef MEMORYJUMP