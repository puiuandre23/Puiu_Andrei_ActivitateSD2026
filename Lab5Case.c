#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCasa {
    int id;
    int nrCamere;
    float pret;
    char* adresa;
    char* numeProprietar;
    unsigned char tip;
};
typedef struct StructuraCasa Casa;

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Casa info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Casa citireCasaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Casa c;

    if (fgets(buffer, 100, file) == NULL) {
        c.id = -1;
        return c;
    }

    aux = strtok(buffer, sep);
    c.id = atoi(aux);
    c.nrCamere = atoi(strtok(NULL, sep));
    c.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    c.adresa = (char*)malloc(strlen(aux) + 1);
    strcpy_s(c.adresa, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    c.numeProprietar = (char*)malloc(strlen(aux) + 1);
    strcpy_s(c.numeProprietar, strlen(aux) + 1, aux);

    c.tip = *strtok(NULL, sep);

    return c;
}

void afisareCasa(Casa c) {
    printf("Id: %d\n", c.id);
    printf("Nr camere: %d\n", c.nrCamere);
    printf("Pret: %.2f\n", c.pret);
    printf("Adresa: %s\n", c.adresa);
    printf("Proprietar: %s\n", c.numeProprietar);
    printf("Tip: %c\n\n", c.tip);
}

void afisareListaCase(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p) {
        afisareCasa(p->info);
        p = p->next;
    }
}

void adaugaCasaInLista(ListaDubla* lista, Casa casaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = casaNoua;
    nou->next = NULL;
    nou->prev = lista->ultim;

    if (lista->ultim) {
        lista->ultim->next = nou;
    }
    else {
        lista->prim = nou;
    }

    lista->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Casa casaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = casaNoua;
    nou->next = lista->prim;
    nou->prev = NULL;

    if (lista->prim) {
        lista->prim->prev = nou;
    }
    else {
        lista->ultim = nou;
    }

    lista->prim = nou;
}

ListaDubla citireLDCaseDinFisier(const char* numeFisier) {
    ListaDubla lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    FILE* f = fopen(numeFisier, "r");
    if (f != NULL) {
        while (1) {
            Casa c = citireCasaDinFisier(f);
            if (c.id == -1) break;
            adaugaCasaInLista(&lista, c);
        }
        fclose(f);
    }

    return lista;
}

void dezalocareLDCase(ListaDubla* lista) {
    Nod* p = lista->prim;
    while (p) {
        Nod* aux = p;
        p = p->next;

        free(aux->info.adresa);
        free(aux->info.numeProprietar);
        free(aux);
    }

    lista->prim = NULL;
    lista->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    Nod* p = lista.prim;
    float suma = 0;
    int contor = 0;

    while (p) {
        suma += p->info.pret;
        contor++;
        p = p->next;
    }

    if (contor == 0) return 0;
    return suma / contor;
}

void stergeCasaDupaID(ListaDubla* lista, int id) {
    Nod* p = lista->prim;

    while (p && p->info.id != id) {
        p = p->next;
    }

    if (!p) return;

    if (p->prev) {
        p->prev->next = p->next;
    }
    else {
        lista->prim = p->next;
    }

    if (p->next) {
        p->next->prev = p->prev;
    }
    else {
        lista->ultim = p->prev;
    }

    free(p->info.adresa);
    free(p->info.numeProprietar);
    free(p);
}

char* getProprietarCasaScumpa(ListaDubla lista) {
    if (!lista.prim) return NULL;

    Nod* max = lista.prim;
    Nod* p = lista.prim->next;

    while (p) {
        if (p->info.pret > max->info.pret) {
            max = p;
        }
        p = p->next;
    }

    char* nume = (char*)malloc(strlen(max->info.numeProprietar) + 1);
    strcpy_s(nume, strlen(max->info.numeProprietar) + 1, max->info.numeProprietar);
    return nume;
}

int main() {
    ListaDubla lista = citireLDCaseDinFisier("Case.txt");

    afisareListaCase(lista);

    printf("Pret mediu: %.2f\n", calculeazaPretMediu(lista));

    char* nume = getProprietarCasaScumpa(lista);
    if (nume) {
        printf("Proprietarul celei mai scumpe case: %s\n", nume);
        free(nume);
    }

    stergeCasaDupaID(&lista, 2);

    afisareListaCase(lista);

    dezalocareLDCase(&lista);

    return 0;
}