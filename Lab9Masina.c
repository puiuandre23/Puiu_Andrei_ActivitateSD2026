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

struct Nod {
    Masina info;
    struct Nod* st;
    struct Nod* dr;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Masina m1;

    fgets(buffer, 100, file);

    aux = strtok(buffer, sep);
    m1.id = atoi(aux);

    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = (float)atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    m1.serie = *strtok(NULL, sep);

    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);

    if (masina.model != NULL) {
        printf("Model: %s\n", masina.model);
    }
    else {
        printf("Model: -\n");
    }

    if (masina.numeSofer != NULL) {
        printf("Nume sofer: %s\n", masina.numeSofer);
    }
    else {
        printf("Nume sofer: -\n");
    }

    printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
    if (*arbore == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = masinaNoua;
        nou->st = NULL;
        nou->dr = NULL;
        *arbore = nou;
    }
    else {
        if (masinaNoua.id < (*arbore)->info.id) {
            adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
        }
        else if (masinaNoua.id > (*arbore)->info.id) {
            adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
        }
    }
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* arbore = NULL;

    if (f != NULL) {
        while (!feof(f)) {
            Masina m = citireMasinaDinFisier(f);
            adaugaMasinaInArbore(&arbore, m);
        }
        fclose(f);
    }
    else {
        printf("Fisierul nu a putut fi deschis.\n");
    }

    return arbore;
}

void afisarePreOrdine(Nod* arbore) {
    if (arbore != NULL) {
        afisareMasina(arbore->info);
        afisarePreOrdine(arbore->st);
        afisarePreOrdine(arbore->dr);
    }
}

void afisareInOrdine(Nod* arbore) {
    if (arbore != NULL) {
        afisareInOrdine(arbore->st);
        afisareMasina(arbore->info);
        afisareInOrdine(arbore->dr);
    }
}

void afisarePostOrdine(Nod* arbore) {
    if (arbore != NULL) {
        afisarePostOrdine(arbore->st);
        afisarePostOrdine(arbore->dr);
        afisareMasina(arbore->info);
    }
}

void afisareMasiniDinArbore(Nod* arbore) {
    printf("Afisare preordine:\n");
    afisarePreOrdine(arbore);

    printf("Afisare inordine:\n");
    afisareInOrdine(arbore);

    printf("Afisare postordine:\n");
    afisarePostOrdine(arbore);
}

void dezalocareArboreDeMasini(Nod** arbore) {
    if (*arbore != NULL) {
        dezalocareArboreDeMasini(&((*arbore)->st));
        dezalocareArboreDeMasini(&((*arbore)->dr));

        free((*arbore)->info.model);
        free((*arbore)->info.numeSofer);
        free(*arbore);
        *arbore = NULL;
    }
}

Masina getMasinaByID(Nod* arbore, int id) {
    if (arbore != NULL) {
        if (id < arbore->info.id) {
            return getMasinaByID(arbore->st, id);
        }
        else if (id > arbore->info.id) {
            return getMasinaByID(arbore->dr, id);
        }
        else {
            Masina m;
            m.id = arbore->info.id;
            m.nrUsi = arbore->info.nrUsi;
            m.pret = arbore->info.pret;
            m.serie = arbore->info.serie;

            m.model = (char*)malloc(strlen(arbore->info.model) + 1);
            strcpy(m.model, arbore->info.model);

            m.numeSofer = (char*)malloc(strlen(arbore->info.numeSofer) + 1);
            strcpy(m.numeSofer, arbore->info.numeSofer);

            return m;
        }
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

int determinaNumarNoduri(Nod* arbore) {
    if (arbore != NULL) {
        return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
    }
    return 0;
}

int calculeazaInaltimeArbore(Nod* arbore) {
    if (arbore != NULL) {
        int inaltimeStanga = calculeazaInaltimeArbore(arbore->st);
        int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);

        if (inaltimeStanga > inaltimeDreapta) {
            return 1 + inaltimeStanga;
        }
        else {
            return 1 + inaltimeDreapta;
        }
    }
    return 0;
}

float calculeazaPretTotal(Nod* arbore) {
    if (arbore != NULL) {
        return arbore->info.pret + calculeazaPretTotal(arbore->st) + calculeazaPretTotal(arbore->dr);
    }
    return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
    if (arbore != NULL) {
        float suma = calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer) +
            calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);

        if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
            suma += arbore->info.pret;
        }

        return suma;
    }
    return 0;
}

int main() {
    Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

    printf("=== AFISARI ARBORE ===\n");
    afisareMasiniDinArbore(arbore);

    printf("=== CAUTARE DUPA ID ===\n");
    Masina masinaCautata = getMasinaByID(arbore, 1);
    afisareMasina(masinaCautata);
    free(masinaCautata.model);
    free(masinaCautata.numeSofer);

    printf("Numar noduri: %d\n", determinaNumarNoduri(arbore));
    printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));
    printf("Pret total: %.2f\n", calculeazaPretTotal(arbore));
    printf("Pret masini sofer Gigel: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Gigel"));

    dezalocareArboreDeMasini(&arbore);

    return 0;
}
