#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraStudent {
    int id;
    int varsta;
    float medie;
    char* nume;
    char* facultate;
    unsigned char grupa;
};
typedef struct StructuraStudent Student;

struct Heap {
    Student* vector;
    int lungime;
    int nrElemente;
};
typedef struct Heap Heap;

Student citireStudentDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);

    char* aux;
    Student s;

    aux = strtok(buffer, sep);
    s.id = atoi(aux);

    s.varsta = atoi(strtok(NULL, sep));
    s.medie = (float)atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    s.nume = (char*)malloc(strlen(aux) + 1);
    strcpy(s.nume, aux);

    aux = strtok(NULL, sep);
    s.facultate = (char*)malloc(strlen(aux) + 1);
    strcpy(s.facultate, aux);

    s.grupa = *strtok(NULL, sep);

    return s;
}

void afisareStudent(Student student) {
    printf("Id: %d\n", student.id);
    printf("Varsta: %d\n", student.varsta);
    printf("Medie: %.2f\n", student.medie);
    printf("Nume: %s\n", student.nume);
    printf("Facultate: %s\n", student.facultate);
    printf("Grupa: %c\n\n", student.grupa);
}

Heap initializareHeap(int lungime) {
    Heap heap;
    heap.lungime = lungime;
    heap.nrElemente = 0;
    heap.vector = (Student*)malloc(sizeof(Student) * lungime);
    return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int pozitieStanga = 2 * pozitieNod + 1;
    int pozitieDreapta = 2 * pozitieNod + 2;
    int pozitieMax = pozitieNod;

    if (pozitieStanga < heap.nrElemente &&
        heap.vector[pozitieStanga].medie > heap.vector[pozitieMax].medie) {
        pozitieMax = pozitieStanga;
    }

    if (pozitieDreapta < heap.nrElemente &&
        heap.vector[pozitieDreapta].medie > heap.vector[pozitieMax].medie) {
        pozitieMax = pozitieDreapta;
    }

    if (pozitieMax != pozitieNod) {
        Student aux = heap.vector[pozitieMax];
        heap.vector[pozitieMax] = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = aux;

        filtreazaHeap(heap, pozitieMax);
    }
}

Heap citireHeapDeStudentiDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Heap heap = initializareHeap(10);

    if (f != NULL) {
        while (!feof(f)) {
            if (heap.nrElemente < heap.lungime) {
                heap.vector[heap.nrElemente] = citireStudentDinFisier(f);
                heap.nrElemente++;
            }
        }

        fclose(f);

        heap.lungime = heap.nrElemente;

        for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
            filtreazaHeap(heap, i);
        }
    }
    else {
        printf("Fisierul nu a putut fi deschis.\n");
    }

    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrElemente; i++) {
        afisareStudent(heap.vector[i]);
    }
}

void afiseazaHeapAscuns(Heap heap) {
    for (int i = heap.nrElemente; i < heap.lungime; i++) {
        afisareStudent(heap.vector[i]);
    }
}

Student extrageStudent(void* heap) {
    Heap* h = (Heap*)heap;

    if (h->nrElemente > 0) {
        Student aux = h->vector[0];

        h->vector[0] = h->vector[h->nrElemente - 1];
        h->vector[h->nrElemente - 1] = aux;

        h->nrElemente--;

        filtreazaHeap(*h, 0);

        return aux;
    }

    Student s;
    s.id = -1;
    s.varsta = 0;
    s.medie = 0;
    s.nume = NULL;
    s.facultate = NULL;
    s.grupa = '-';
    return s;
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->lungime; i++) {
        free(heap->vector[i].nume);
        free(heap->vector[i].facultate);
    }

    free(heap->vector);
    heap->vector = NULL;
    heap->lungime = 0;
    heap->nrElemente = 0;
}

int main() {
    Heap heap = citireHeapDeStudentiDinFisier("studenti.txt");

    printf("Elementele din heap sunt:\n");
    afisareHeap(heap);

    printf("\nStudent extras:\n");
    Student s = extrageStudent(&heap);
    afisareStudent(s);

    printf("\nElementele vizibile ramase in heap sunt:\n");
    afisareHeap(heap);

    printf("\nElementele ascunse din heap sunt:\n");
    afiseazaHeapAscuns(heap);

    dezalocareHeap(&heap);

    return 0;
}
