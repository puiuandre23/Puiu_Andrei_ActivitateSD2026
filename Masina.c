#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Masina {
    int id;
    int nrUsi;
    char* model;     
    float pret;
    char serie;
};

struct Masina initializare(int id, int nrUsi, const char* model, float pret, char serie) {
    struct Masina m;
    m.id = id;
    m.nrUsi = nrUsi;

    m.model = (char*)malloc(strlen(model) + 1);
    strcpy_s(m.model, strlen(model) + 1, model);

    m.pret = pret;
    m.serie = serie;
    return m;
}

void afisare(struct Masina m) {
    if (m.model != NULL) {
        printf("%d. Masina %s seria %c are %d usi si costa %5.2f EUR.\n",
            m.id, m.model, m.serie, m.nrUsi, m.pret);
    }
    else {
        printf("%d. Masina seria %c are %d usi si costa %5.2f EUR.\n",
            m.id, m.serie, m.nrUsi, m.pret);
    }
}

void afisareVector(struct Masina* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
    struct Masina* vectorNou = NULL;

    if (vector == NULL || nrElemente <= 0 || nrElementeCopiate <= 0) {
        return NULL;
    }
    if (nrElementeCopiate > nrElemente) {
        nrElementeCopiate = nrElemente;
    }

    vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * nrElementeCopiate);

    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = vector[i];
       
        vectorNou[i].model = (char*)malloc(strlen(vector[i].model) + 1);
        strcpy_s(vectorNou[i].model, strlen(vector[i].model) + 1, vector[i].model);
    }

    return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
    if (vector == NULL || *vector == NULL || nrElemente == NULL) return;

    for (int i = 0; i < (*nrElemente); i++) {
        if ((*vector)[i].model != NULL) {
            free((*vector)[i].model);
            (*vector)[i].model = NULL;
        }
    }

    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}


void copiazaMasiniScumpe(struct Masina* vector, char nrElemente, float pretMinim,
    struct Masina** vectorNou, int* dimensiune) {

    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret >= pretMinim) {
            (*dimensiune)++;
        }
    }

    if (*vectorNou != NULL) {
        free(*vectorNou);    
        *vectorNou = NULL;
    }

    *vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * (*dimensiune));

    int k = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret >= pretMinim) {
            (*vectorNou)[k] = vector[i];
            
            (*vectorNou)[k].model = (char*)malloc(strlen(vector[i].model) + 1);
            strcpy_s((*vectorNou)[k].model, strlen(vector[i].model) + 1, vector[i].model);
            k++;
        }
    }
}


struct Masina getPrimaMasinaByModel(struct Masina* vector, int nrElemente, const char* model) {
    struct Masina m;
    m.id = -1;
    m.nrUsi = 0;
    m.pret = 0;
    m.serie = '-';
    m.model = NULL;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].model != NULL && strcmp(vector[i].model, model) == 0) {
            m = vector[i];
            m.model = (char*)malloc(strlen(vector[i].model) + 1);
            strcpy_s(m.model, strlen(vector[i].model) + 1, vector[i].model);
            return m;
        }
    }

    return m;
}

int main() {
    struct Masina* masini = NULL;
    int nrMasini = 3;

    masini = (struct Masina*)malloc(sizeof(struct Masina) * nrMasini);

    masini[0] = initializare(1, 4, "BMW", 25000, 'B');
    masini[1] = initializare(2, 2, "Audi", 22000, 'A');
    masini[2] = initializare(3, 4, "Dacia", 9000, 'D');

    printf("Vector initial:\n");
    afisareVector(masini, nrMasini);

    struct Masina* primeleMasini = NULL;
    int nrPrimeleMasini = 2;

    primeleMasini = copiazaPrimeleNElemente(masini, nrMasini, nrPrimeleMasini);
    printf("\n\nPrimele masini:\n");
    afisareVector(primeleMasini, nrPrimeleMasini);
    dezalocare(&primeleMasini, &nrPrimeleMasini);

    struct Masina* masiniScumpe = NULL;
    int nrMasiniScumpe = 0;

    copiazaMasiniScumpe(masini, (char)nrMasini, 20000, &masiniScumpe, &nrMasiniScumpe);
    printf("\n\nMasini scumpe (>= 20000):\n");
    afisareVector(masiniScumpe, nrMasiniScumpe);
    dezalocare(&masiniScumpe, &nrMasiniScumpe);

    struct Masina masinaGasita = getPrimaMasinaByModel(masini, nrMasini, "Dacia");
    printf("\n\nMasina gasita:\n");
    afisare(masinaGasita);

    if (masinaGasita.model != NULL) {
        free(masinaGasita.model);
        masinaGasita.model = NULL;
    }

    dezalocare(&masini, &nrMasini);

    return 0;
}