#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCasa {
    int id;
    int nrCamere;
    float pret;
    char* adresa;
    char* proprietar;
    char clasa;
};
typedef struct StructuraCasa Casa;

struct Nod {
    Casa info;
    struct Nod* next;
};
typedef struct Nod Nod;

Casa citireCasaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);

    Casa c;
    char* aux;

    aux = strtok(buffer, sep);
    c.id = atoi(aux);

    c.nrCamere = atoi(strtok(NULL, sep));
    c.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    c.adresa = (char*)malloc(strlen(aux) + 1);
    strcpy(c.adresa, aux);

    aux = strtok(NULL, sep);
    c.proprietar = (char*)malloc(strlen(aux) + 1);
    strcpy(c.proprietar, aux);

    c.clasa = *strtok(NULL, sep);

    return c;
}

void afisareCasa(Casa casa) {
    printf("ID: %d\n", casa.id);
    printf("Nr camere: %d\n", casa.nrCamere);
    printf("Pret: %.2f\n", casa.pret);
    printf("Adresa: %s\n", casa.adresa);
    printf("Proprietar: %s\n", casa.proprietar);
    printf("Clasa: %c\n\n", casa.clasa);
}

void afisareListaCase(Nod* cap) {
    while (cap) {
        afisareCasa(cap->info);
        cap = cap->next;
    }
}

void adaugaCasaInLista(Nod** cap, Casa casaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = casaNoua;
    nou->next = NULL;

    if (*cap) {
        Nod* p = *cap;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
    else {
        *cap = nou;
    }
}

Nod* citireListaCaseDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* cap = NULL;

    if (f) {
        while (!feof(f)) {
            adaugaCasaInLista(&cap, citireCasaDinFisier(f));
        }
        fclose(f);
    }

    return cap;
}

void dezalocareListaCase(Nod** cap) {
    while (*cap) {
        Nod* p = *cap;
        *cap = p->next;

        free(p->info.adresa);
        free(p->info.proprietar);
        free(p);
    }
}

float calculeazaPretMediu(Nod* cap) {
    float suma = 0;
    int nr = 0;

    while (cap) {
        suma += cap->info.pret;
        nr++;
        cap = cap->next;
    }

    if (nr > 0)
        return suma / nr;
    return 0;
}

int main() {

    Nod* cap = citireListaCaseDinFisier("Case.txt");

    printf("Lista initiala de case:\n\n");
    afisareListaCase(cap);

    printf("Pret mediu case: %.2f\n\n", calculeazaPretMediu(cap));

    dezalocareListaCase(&cap);

    return 0;
}