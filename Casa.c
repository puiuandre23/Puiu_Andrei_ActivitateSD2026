#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Casa {
    int id;
    int anConstructie;
    char* strada;
    float pret;
    char clasaEnergetica;
};

struct Casa initializare(int id, int anConstructie, const char* strada, float pret, char clasaEnergetica) {
    struct Casa c;
    c.id = id;
    c.anConstructie = anConstructie;

    c.strada = (char*)malloc(strlen(strada) + 1);
    strcpy_s(c.strada, strlen(strada) + 1, strada);

    c.pret = pret;
    c.clasaEnergetica = clasaEnergetica;
    return c;
}

void afisare(struct Casa c) {
    if (c.strada != NULL) {
        printf("%d. Casa din strada %s a fost construita in anul %d si costa %5.2f EUR avand clasa energetica %c.\n",
            c.id, c.strada, c.anConstructie, c.pret, c.clasaEnergetica);
    }
    else {
        printf("%d. Casa a fost construita in anul %d si costa %5.2f EUR avand clasa energetica %c.\n",
            c.id, c.anConstructie, c.pret, c.clasaEnergetica);
    }
}

void afisareVector(struct Casa* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct Casa* copiazaPrimeleNElemente(struct Casa* vector, int nrElemente, int nrElementeCopiate) {
    struct Casa* vectorNou = NULL;

    if (nrElementeCopiate <= 0 || nrElemente <= 0 || vector == NULL) {
        return NULL;
    }

    if (nrElementeCopiate > nrElemente) {
        nrElementeCopiate = nrElemente;
    }

    vectorNou = (struct Casa*)malloc(sizeof(struct Casa) * nrElementeCopiate);

    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = vector[i];

         vectorNou[i].strada = (char*)malloc(strlen(vector[i].strada) + 1);
        strcpy_s(vectorNou[i].strada, strlen(vector[i].strada) + 1, vector[i].strada);
    }

    return vectorNou;
}

void dezalocare(struct Casa** vector, int* nrElemente) {
    if (vector == NULL || *vector == NULL || nrElemente == NULL) return;

    for (int i = 0; i < (*nrElemente); i++) {
        if ((*vector)[i].strada != NULL) {
            free((*vector)[i].strada);
            (*vector)[i].strada = NULL;
        }
    }

    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}

void copiazaCaseScumpe(struct Casa* vector, char nrElemente, float pretMinim, struct Casa** vectorNou, int* dimensiune) {
    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret >= pretMinim) {
            (*dimensiune)++;
        }
    }

  
    if (*vectorNou != NULL) {
        int dimVeche = *dimensiune; 
        free(*vectorNou);
        *vectorNou = NULL;
    }

    *vectorNou = (struct Casa*)malloc(sizeof(struct Casa) * (*dimensiune));

    int k = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret >= pretMinim) {
            (*vectorNou)[k] = vector[i];

            (*vectorNou)[k].strada = (char*)malloc(strlen(vector[i].strada) + 1);
            strcpy_s((*vectorNou)[k].strada, strlen(vector[i].strada) + 1, vector[i].strada);

            k++;
        }
    }
}

struct Casa getPrimaCasaByStrada(struct Casa* vector, int nrElemente, const char* strada) {
    struct Casa c;
    c.id = -1;
    c.anConstructie = 0;
    c.pret = 0;
    c.clasaEnergetica = '-';
    c.strada = NULL;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].strada != NULL && strcmp(vector[i].strada, strada) == 0) {
            c = vector[i];
            c.strada = (char*)malloc(strlen(vector[i].strada) + 1);
            strcpy_s(c.strada, strlen(vector[i].strada) + 1, vector[i].strada);
            return c;
        }
    }

    return c;
}

int main() {
    struct Casa* caseVector = NULL;
    int nrCase = 3;

    caseVector = (struct Casa*)malloc(sizeof(struct Casa) * nrCase);

    caseVector[0] = initializare(1, 2024, "StradaX", 200000, 'A');
    caseVector[1] = initializare(2, 2025, "StradaY", 300000, 'B');
    caseVector[2] = initializare(3, 2026, "StradaZ", 400000, 'C');

    printf("Vector initial:\n");
    afisareVector(caseVector, nrCase);

    struct Casa* primeleCase = NULL;
    int nrPrimeleCase = 2;

    primeleCase = copiazaPrimeleNElemente(caseVector, nrCase, nrPrimeleCase);
    printf("\n\nPrimele case:\n");
    afisareVector(primeleCase, nrPrimeleCase);

    dezalocare(&primeleCase, &nrPrimeleCase);

    struct Casa* caseScumpe = NULL;
    int nrCaseScumpe = 0;
    copiazaCaseScumpe(caseVector, (char)nrCase, 250000, &caseScumpe, &nrCaseScumpe);

    printf("\n\nCase scumpe (>= 250000):\n");
    afisareVector(caseScumpe, nrCaseScumpe);

    dezalocare(&caseScumpe, &nrCaseScumpe);

     struct Casa casaGasita = getPrimaCasaByStrada(caseVector, nrCase, "StradaZ");
    printf("\n\nCasa gasita:\n");
    afisare(casaGasita);

    if (casaGasita.strada != NULL) {
        free(casaGasita.strada);
        casaGasita.strada = NULL;
    }

    dezalocare(&caseVector, &nrCase);

    return 0;
}