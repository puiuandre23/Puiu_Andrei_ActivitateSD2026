#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    int id;
    int varsta;
    float medie;
    char* nume;
    char* facultate;
    char grupa;
} Student;

typedef struct Heap {
    Student* vector;
    int nrStudenti;
    int lungime;
} Heap;

Student citireStudentDinFisier(FILE* f) {
    Student s;
    char buffer[100];
    char sep[3] = ",\n";

    fgets(buffer, 100, f);

    char* aux = strtok(buffer, sep);
    s.id = atoi(aux);

    aux = strtok(NULL, sep);
    s.varsta = atoi(aux);

    aux = strtok(NULL, sep);
    s.medie = (float)atof(aux);

    aux = strtok(NULL, sep);
    s.nume = (char*)malloc(strlen(aux) + 1);
    strcpy(s.nume, aux);

    aux = strtok(NULL, sep);
    s.facultate = (char*)malloc(strlen(aux) + 1);
    strcpy(s.facultate, aux);

    aux = strtok(NULL, sep);
    s.grupa = aux[0];

    return s;
}

void afisareStudent(Student s) {
    printf("\nId: %d", s.id);
    printf("\nVarsta: %d", s.varsta);
    printf("\nMedie: %.2f", s.medie);
    printf("\nNume: %s", s.nume);
    printf("\nFacultate: %s", s.facultate);
    printf("\nGrupa: %c\n", s.grupa);
}

void filtreazaHeap(Heap heap, int poz) {
    int st = 2 * poz + 1;
    int dr = 2 * poz + 2;
    int max = poz;

    if (st < heap.nrStudenti &&
        heap.vector[st].medie > heap.vector[max].medie) {
        max = st;
    }

    if (dr < heap.nrStudenti &&
        heap.vector[dr].medie > heap.vector[max].medie) {
        max = dr;
    }

    if (max != poz) {
        Student aux = heap.vector[max];
        heap.vector[max] = heap.vector[poz];
        heap.vector[poz] = aux;

        filtreazaHeap(heap, max);
    }
}


Heap citireHeapDinFisier(const char* numeFisier) {
    Heap h;
    FILE* f = fopen(numeFisier, "r");

    h.vector = (Student*)malloc(sizeof(Student) * 10);
    h.nrStudenti = 0;
    h.lungime = 10;

    while (!feof(f)) {
        h.vector[h.nrStudenti] = citireStudentDinFisier(f);
        h.nrStudenti++;
    }

    fclose(f);

    h.lungime = h.nrStudenti;

    for (int i = (h.nrStudenti - 1) / 2; i >= 0; i--) {
        filtreazaHeap(h, i);
    }

    return h;
}


void afisareHeap(Heap h) {
    for (int i = 0; i < h.nrStudenti; i++) {
        afisareStudent(h.vector[i]);
    }
}


Student extrageStudent(Heap* h) {
    Student s = h->vector[0];

    h->vector[0] = h->vector[h->nrStudenti - 1];
    h->nrStudenti--;

    filtreazaHeap(*h, 0);

    return s;
}


void dezalocareHeap(Heap* h) {
    for (int i = 0; i < h->nrStudenti; i++) {
        free(h->vector[i].nume);
        free(h->vector[i].facultate);
    }
    free(h->vector);
}

int main() {
    Heap h = citireHeapDinFisier("studenti.txt");

    printf("\n--- HEAP ---\n");
    afisareHeap(h);

    printf("\n--- EXTRAGERE ---\n");
    while (h.nrStudenti > 0) {
        Student s = extrageStudent(&h);
        afisareStudent(s);

        free(s.nume);
        free(s.facultate);
    }

    return 0;
}