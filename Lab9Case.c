#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Casa {
    int id;
    int nrCamere;
    float pret;
    char* adresa;
    char* proprietar;
    char serie;
} Casa;

typedef struct Nod {
    Casa info;
    struct Nod* stanga;
    struct Nod* dreapta;
} Nod;


Casa citireCasaDinFisier(FILE* f) {
    Casa c;
    char buffer[100];
    char sep[3] = ",\n";

    fgets(buffer, 100, f);

    char* aux = strtok(buffer, sep);
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
    printf("\nId: %d", c.id);
    printf("\nCamere: %d", c.nrCamere);
    printf("\nPret: %.2f", c.pret);
    printf("\nAdresa: %s", c.adresa);
    printf("\nProprietar: %s", c.proprietar);
    printf("\nSerie: %c\n", c.serie);
}


Nod* adaugaCasaInArbore(Nod* radacina, Casa c) {
    if (radacina == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = c;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        return nou;
    }

    if (c.id < radacina->info.id)
        radacina->stanga = adaugaCasaInArbore(radacina->stanga, c);
    else
        radacina->dreapta = adaugaCasaInArbore(radacina->dreapta, c);

    return radacina;
}

Nod* citireArboreDeCaseDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* radacina = NULL;

    while (!feof(f)) {
        Casa c = citireCasaDinFisier(f);
        radacina = adaugaCasaInArbore(radacina, c);
    }

    fclose(f);
    return radacina;
}


void afisarePreordine(Nod* r) {
    if (r) {
        afisareCasa(r->info);
        afisarePreordine(r->stanga);
        afisarePreordine(r->dreapta);
    }
}


void afisareInordine(Nod* r) {
    if (r) {
        afisareInordine(r->stanga);
        afisareCasa(r->info);
        afisareInordine(r->dreapta);
    }
}


void afisarePostordine(Nod* r) {
    if (r) {
        afisarePostordine(r->stanga);
        afisarePostordine(r->dreapta);
        afisareCasa(r->info);
    }
}


int determinaNumarNoduri(Nod* r) {
    if (!r) return 0;
    return 1 + determinaNumarNoduri(r->stanga) + determinaNumarNoduri(r->dreapta);
}


int calculeazaInaltimeArbore(Nod* r) {
    if (!r) return 0;

    int st = calculeazaInaltimeArbore(r->stanga);
    int dr = calculeazaInaltimeArbore(r->dreapta);

    return 1 + (st > dr ? st : dr);
}


float calculeazaPretTotal(Nod* r) {
    if (!r) return 0;

    return r->info.pret +
        calculeazaPretTotal(r->stanga) +
        calculeazaPretTotal(r->dreapta);
}


Casa getCasaByID(Nod* r, int id) {
    if (!r) {
        Casa c;
        c.id = -1;
        return c;
    }

    if (r->info.id == id)
        return r->info;

    if (id < r->info.id)
        return getCasaByID(r->stanga, id);
    else
        return getCasaByID(r->dreapta, id);
}


float calculeazaPretCaseProprietar(Nod* r, const char* nume) {
    if (!r) return 0;

    float suma = 0;

    if (strcmp(r->info.proprietar, nume) == 0)
        suma += r->info.pret;

    suma += calculeazaPretCaseProprietar(r->stanga, nume);
    suma += calculeazaPretCaseProprietar(r->dreapta, nume);

    return suma;
}


void dezalocareArbore(Nod** r) {
    if (*r) {
        dezalocareArbore(&(*r)->stanga);
        dezalocareArbore(&(*r)->dreapta);

        free((*r)->info.adresa);
        free((*r)->info.proprietar);
        free(*r);
        *r = NULL;
    }
}

int main() {
    Nod* radacina = citireArboreDeCaseDinFisier("Case.txt");

    printf("\nPREORDINE:\n");
    afisarePreordine(radacina);

    printf("\nINORDINE:\n");
    afisareInordine(radacina);

    printf("\nPOSTORDINE:\n");
    afisarePostordine(radacina);

    printf("\nNumar noduri: %d", determinaNumarNoduri(radacina));
    printf("\nInaltime: %d", calculeazaInaltimeArbore(radacina));
    printf("\nPret total: %.2f", calculeazaPretTotal(radacina));

    printf("\nPret pentru Popescu: %.2f",
        calculeazaPretCaseProprietar(radacina, "Popescu"));

    dezalocareArbore(&radacina);

    return 0;
}