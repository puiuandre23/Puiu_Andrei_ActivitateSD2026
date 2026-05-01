#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Casa {
    int id;
    int nrCamere;
    float pret;
    char* adresa;
    char* proprietar;
    char serie;
};
typedef struct Casa Casa;

struct Heap {
    Casa* vector;
    int lungime;
    int nrElemente;
};
typedef struct Heap Heap;

Casa citireCasaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Casa c;

    fgets(buffer, 100, file);

    aux = strtok(buffer, sep);
    c.id = atoi(aux);

    aux = strtok(NULL, sep);
    c.nrCamere = atoi(aux);

    aux = strtok(NULL, sep);
    c.pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    c.adresa = (char*)malloc(strlen(aux) + 1);
    strcpy(c.adresa, aux);

    aux = strtok(NULL, sep);
    c.proprietar = (char*)malloc(strlen(aux) + 1);
    strcpy(c.proprietar, aux);

    aux = strtok(NULL, sep);
    c.serie = aux[0];

    return c;
}

void afisareCasa(Casa c) {
    printf("Id: %d\n", c.id);
    printf("Nr camere: %d\n", c.nrCamere);
    printf("Pret: %.2f\n", c.pret);
    printf("Adresa: %s\n", c.adresa);
    printf("Proprietar: %s\n", c.proprietar);
    printf("Serie: %c\n\n", c.serie);
}

Heap initializareHeap(int lungime) {
    Heap heap;
    heap.lungime = lungime;
    heap.nrElemente = 0;
    heap.vector = (Casa*)malloc(sizeof(Casa) * lungime);
    return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int st = 2 * pozitieNod + 1;
    int dr = 2 * pozitieNod + 2;
    int max = pozitieNod;

    if (st < heap.nrElemente &&
        heap.vector[st].pret > heap.vector[max].pret) {
        max = st;
    }

    if (dr < heap.nrElemente &&
        heap.vector[dr].pret > heap.vector[max].pret) {
        max = dr;
    }

    if (max != pozitieNod) {
        Casa aux = heap.vector[max];
        heap.vector[max] = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = aux;

        filtreazaHeap(heap, max);
    }
}

Heap citireHeapDeCaseDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Heap heap = initializareHeap(10);

    if (f != NULL) {
        while (!feof(f)) {
            if (heap.nrElemente < heap.lungime) {
                heap.vector[heap.nrElemente] = citireCasaDinFisier(f);
                heap.nrElemente++;
            }
        }

        fclose(f);

        heap.lungime = heap.nrElemente;

        for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
            filtreazaHeap(heap, i);
        }
    }

    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrElemente; i++) {
        afisareCasa(heap.vector[i]);
    }
}

void afiseazaHeapAscuns(Heap heap) {
    for (int i = heap.nrElemente; i < heap.lungime; i++) {
        afisareCasa(heap.vector[i]);
    }
}

Casa extrageCasa(void* heap) {
    Heap* h = (Heap*)heap;

    if (h->nrElemente > 0) {
        Casa aux = h->vector[0];

        h->vector[0] = h->vector[h->nrElemente - 1];
        h->vector[h->nrElemente - 1] = aux;

        h->nrElemente--;

        filtreazaHeap(*h, 0);

        return aux;
    }

    Casa c;
    c.id = -1;
    c.nrCamere = 0;
    c.pret = 0;
    c.adresa = NULL;
    c.proprietar = NULL;
    c.serie = '-';
    return c;
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->lungime; i++) {
        free(heap->vector[i].adresa);
        free(heap->vector[i].proprietar);
    }

    free(heap->vector);
    heap->vector = NULL;
    heap->lungime = 0;
    heap->nrElemente = 0;
}

int main() {
    Heap heap = citireHeapDeCaseDinFisier("Case.txt");

    printf("HEAP:\n");
    afisareHeap(heap);

    printf("\nEXTRAGERE:\n");
    Casa c = extrageCasa(&heap);
    afisareCasa(c);

    printf("\nHEAP RAMAS:\n");
    afisareHeap(heap);

    printf("\nElementele ascunse din heap sunt:\n");
    afiseazaHeapAscuns(heap);

    dezalocareHeap(&heap);

    return 0;
}