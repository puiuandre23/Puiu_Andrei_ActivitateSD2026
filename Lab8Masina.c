#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Heap {
    Masina* vector;
    int nrMasini;
    int lungime;
};
typedef struct Heap Heap;

struct Nod {
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;

Masina citireMasinaDinLinie(char* linie) {
    char sep[3] = ",\n";
    char* aux;
    Masina m1;

    aux = strtok(linie, sep);
    m1.id = atoi(aux);

    aux = strtok(NULL, sep);
    m1.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    m1.pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    aux = strtok(NULL, sep);
    m1.serie = aux[0];

    return m1;
}

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    fgets(buffer, 100, file);
    return citireMasinaDinLinie(buffer);
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
    Heap h;
    h.lungime = lungime;
    h.nrMasini = 0;
    h.vector = (Masina*)malloc(sizeof(Masina) * lungime);
    return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int pozS = 2 * pozitieNod + 1;
    int pozD = 2 * pozitieNod + 2;
    int pozMax = pozitieNod;

    if (pozS < heap.nrMasini && heap.vector[pozS].id > heap.vector[pozMax].id) {
        pozMax = pozS;
    }

    if (pozD < heap.nrMasini && heap.vector[pozD].id > heap.vector[pozMax].id) {
        pozMax = pozD;
    }

    if (pozMax != pozitieNod) {
        Masina aux = heap.vector[pozMax];
        heap.vector[pozMax] = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = aux;

        filtreazaHeap(heap, pozMax);
    }
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Heap h = initializareHeap(10);

    if (f != NULL) {
        char buffer[100];

        while (fgets(buffer, 100, f) != NULL) {
            if (h.nrMasini < h.lungime) {
                h.vector[h.nrMasini] = citireMasinaDinLinie(buffer);
                h.nrMasini++;
            }
        }

        fclose(f);

        h.lungime = h.nrMasini;

        for (int i = (h.nrMasini - 2) / 2; i >= 0; i--) {
            filtreazaHeap(h, i);
        }
    }
    else {
        printf("Fisierul nu a putut fi deschis.\n");
    }

    return h;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrMasini; i++) {
        afisareMasina(heap.vector[i]);
    }
}

void afiseazaHeapAscuns(Heap heap) {
    for (int i = heap.nrMasini; i < heap.lungime; i++) {
        afisareMasina(heap.vector[i]);
    }
}

Masina extrageMasina(Heap* heap) {
    if (heap->nrMasini > 0) {
        Masina aux = heap->vector[0];
        heap->vector[0] = heap->vector[heap->nrMasini - 1];
        heap->vector[heap->nrMasini - 1] = aux;

        heap->nrMasini--;

        for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--) {
            filtreazaHeap(*heap, i);
        }

        return aux;
    }

    Masina m;
    m.id = -1;
    m.nrUsi = 0;
    m.pret = 0;
    m.model = NULL;
    m.numeSofer = NULL;
    m.serie = '-';
    return m;
}

void inserareInLista(Nod** lista, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = m;
    nou->next = *lista;
    *lista = nou;
}

void afisareLista(Nod* lista) {
    while (lista != NULL) {
        afisareMasina(lista->info);
        lista = lista->next;
    }
}

Nod* masiniCuIDMare(Heap* heap, int idMinim) {
    Nod* lista = NULL;

    while (heap->nrMasini > 0 && heap->vector[0].id >= idMinim) {
        Masina m = extrageMasina(heap);
        inserareInLista(&lista, m);
    }

    return lista;
}

void dezalocareLista(Nod** lista) {
    while (*lista != NULL) {
        Nod* aux = *lista;
        *lista = (*lista)->next;
        free(aux);
    }
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->lungime; i++) {
        free(heap->vector[i].model);
        free(heap->vector[i].numeSofer);
    }

    free(heap->vector);
    heap->vector = NULL;
    heap->lungime = 0;
    heap->nrMasini = 0;
}

int main() {
    Heap h = citireHeapDeMasiniDinFisier("masini.txt");

    printf("Elementele vizibile din heap sunt:\n");
    afisareHeap(h);

    Nod* lista = masiniCuIDMare(&h, 4);

    printf("\nLista masinilor extrase cu id >= 4:\n");
    afisareLista(lista);

    printf("\nElementele vizibile ramase in heap sunt:\n");
    afisareHeap(h);

    printf("\nElementele ascunse din heap sunt:\n");
    afiseazaHeapAscuns(h);

    dezalocareLista(&lista);
    dezalocareHeap(&h);

    return 0;
}
