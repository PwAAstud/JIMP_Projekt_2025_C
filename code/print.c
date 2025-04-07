#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"..\header\print.h"
#include"..\header\graf.h"
#define memoryJump 16


void prit(wczytanie* writ, int t, int b){
    printf("%s\n", writ->linia1);
    printf("%s\n", writ->linia2);
    printf("%s\n", writ->linia3);
    int i, p, pomoc=0, ktore=0;
    long tab[writ->rozmiar];
    //sortGrafData(writ->nodes, writ->rozmiar);
    if(t!=1 || b!=0){
       return;
    }
    for(i=0; i<writ->rozmiar; i++){
        p=0;
        for (long j = 0; j < writ->nodes[i]->n; j++) {
            if (writ->nodes[i]->conetion[j]->id > writ->nodes[i]->id)
                p=1;
        }
        if(p!=1)
        {continue;}

        printf("%ld;", writ->nodes[i]->id);
        tab[ktore]=pomoc;
        ktore++;
        pomoc++;
        for (long j = 0; j < writ->nodes[i]->n&&writ->nodes[i]->conetion[j]->id > writ->nodes[i]->id; j++) {
            if (writ->nodes[i]->conetion[j] != NULL) {
                printf("%ld;",writ->nodes[i]->conetion[j]->id);
                pomoc++;
            }
        }

    }
    printf("\n");

    for(i=0; i<=ktore; i++)
    {
        printf("%ld;",tab[i]);
    }

      printf("\n");
}

node* init_node(long id, long n) {
    node* new_node = (node*)malloc(sizeof(node));  // alokacja pamiêci na wêze³
    if (new_node == NULL) {
        printf("Blad alokacji pamieci!\n");
        exit(1);
    }

    new_node->id = id;  // ustawienie identyfikatora
    new_node->n = n;    // ustawienie liczby po³¹czeñ
    new_node->conetion = (node**)malloc(memoryJump * sizeof(node*));  // alokacja pamiêci na po³¹czenia
    if (new_node->conetion == NULL) {
        printf("Blad alokacji pamieci na polaczenia!\n");
        free(new_node);  // zwolnienie wczeœniej zaalokowanej pamiêci
        exit(1);
    }

    // Inicjalizacja po³¹czeñ jako NULL
    for (long i = 0; i < n; i++) {
        new_node->conetion[i] = NULL;
    }

    return new_node;
}

void add_connection(node* from, node* to) {

    from->conetion[from->n] = to;
    from->n++;

    if(from->n%memoryJump==0)
    {
        from->conetion = realloc(from->conetion,(from->n+memoryJump) * sizeof(node*));
    }

    to->conetion[to->n] = from;
    to->n++;

    if(to->n%memoryJump==0)
    {
        to->conetion = realloc(to->conetion,(to->n+memoryJump) * sizeof(node*));
    }


}


wczytanie* scan(char* plikw){
    long MAX_LINIA = 1000000;
       long ROZMIAR_TABLICY = 1000000;
       long n=0;
       long i=0;
    FILE *plik;
    char linia[MAX_LINIA];
    long liczby1[ROZMIAR_TABLICY], liczby2[ROZMIAR_TABLICY];
    long licznik1 = 0, licznik2 = 0;
    wczytanie* red= (wczytanie*)malloc(sizeof(wczytanie));

    // Otwórz plik do odczytu
    plik = fopen(plikw, "r");
    if (plik == NULL) {
        printf("Nie mo¿na otworzyæ pliku\n");
        return red;
    }

    // Wczytanie pierwszych 3 linijek tekstu
    if (fgets(linia, MAX_LINIA, plik)) {
        linia[strcspn(linia, "\n")] = '\0';
         red->linia1 = (char*)malloc(strlen(linia)*sizeof(char));
        strcpy(red->linia1, linia);
    }
    if (fgets(linia, MAX_LINIA, plik)) {
        linia[strcspn(linia, "\n")] = '\0';
        red->linia2 = (char*)malloc(strlen(linia)*sizeof(char));
        strcpy(red->linia2, linia);
    }
    if (fgets(linia, MAX_LINIA, plik)) {
        linia[strcspn(linia, "\n")] = '\0';
        red->linia3 =(char*)malloc(strlen(linia)*sizeof (char));
        strcpy(red->linia3, linia);
    }

    // Wczytanie 4-tej linijki
    if (fgets(linia, MAX_LINIA, plik)) {
        char *token = strtok(linia, ";");
        while (token != NULL) {
            liczby1[licznik1++] = atoi(token);
            token = strtok(NULL, ";");
                   i=(i>=liczby1[licznik1])? i : liczby1[licznik1]+1;
        }
    }
    red->rozmiar=i;
    red->nodes = (node**)malloc(i * sizeof(node*));
    long j;
    for(j=0; j<i;j++)
        {
            red->nodes[j]=init_node(j, 0);
        }
    // Wczytanie 5-tej linijki
    if (fgets(linia, MAX_LINIA, plik)) {
        char *token = strtok(linia, ";");
        liczby2[licznik2++] = atoi(token);
            token = strtok(NULL, ";");
        while (token != NULL) {
            liczby2[licznik2++] = atoi(token);
            token = strtok(NULL, ";");


            for(j=1; j<(liczby2[licznik2]-liczby2[licznik2-1]); j++)
            {

                add_connection(red->nodes[liczby1[liczby2[licznik2-1]]], red->nodes[liczby1[liczby2[licznik2-1]]+j]);
            }
        }

    }


    return red;
}

