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

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Student info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Student citireStudentDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Student s;

    if (fgets(buffer, 100, file) == NULL) {
        s.id = -1;
        return s;
    }

    aux = strtok(buffer, sep);
    s.id = atoi(aux);
    s.varsta = atoi(strtok(NULL, sep));
    s.medie = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    s.nume = (char*)malloc(strlen(aux) + 1);
    strcpy_s(s.nume, strlen(aux) + 1, aux);

    aux = strtok(NULL, sep);
    s.facultate = (char*)malloc(strlen(aux) + 1);
    strcpy_s(s.facultate, strlen(aux) + 1, aux);

    s.grupa = *strtok(NULL, sep);

    return s;
}

void afisareStudent(Student s) {
    printf("Id: %d\n", s.id);
    printf("Varsta: %d\n", s.varsta);
    printf("Medie: %.2f\n", s.medie);
    printf("Nume: %s\n", s.nume);
    printf("Facultate: %s\n", s.facultate);
    printf("Grupa: %c\n\n", s.grupa);
}

void afisareListaStudenti(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p) {
        afisareStudent(p->info);
        p = p->next;
    }
}

void adaugaStudentInLista(ListaDubla* lista, Student studentNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = studentNou;
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

void adaugaLaInceputInLista(ListaDubla* lista, Student studentNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = studentNou;
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

ListaDubla citireLDStudentiDinFisier(const char* numeFisier) {
    ListaDubla lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    FILE* f = fopen(numeFisier, "r");
    if (f != NULL) {
        while (1) {
            Student s = citireStudentDinFisier(f);
            if (s.id == -1) {
                break;
            }
            adaugaStudentInLista(&lista, s);
        }
        fclose(f);
    }

    return lista;
}

void dezalocareLDStudenti(ListaDubla* lista) {
    Nod* p = lista->prim;
    while (p) {
        Nod* aux = p;
        p = p->next;

        free(aux->info.nume);
        free(aux->info.facultate);
        free(aux);
    }

    lista->prim = NULL;
    lista->ultim = NULL;
}

float calculeazaMedieGenerala(ListaDubla lista) {
    Nod* p = lista.prim;
    float suma = 0;
    int contor = 0;

    while (p) {
        suma += p->info.medie;
        contor++;
        p = p->next;
    }

    if (contor == 0) return 0;
    return suma / contor;
}

void stergeStudentDupaID(ListaDubla* lista, int id) {
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

    free(p->info.nume);
    free(p->info.facultate);
    free(p);
}

char* getNumeStudentCuMedieMaxima(ListaDubla lista) {
    if (!lista.prim) return NULL;

    Nod* max = lista.prim;
    Nod* p = lista.prim->next;

    while (p) {
        if (p->info.medie > max->info.medie) {
            max = p;
        }
        p = p->next;
    }

    char* nume = (char*)malloc(strlen(max->info.nume) + 1);
    strcpy_s(nume, strlen(max->info.nume) + 1, max->info.nume);
    return nume;
}

int main() {
    ListaDubla lista = citireLDStudentiDinFisier("studenti.txt");

    afisareListaStudenti(lista);

    printf("Media generala: %.2f\n", calculeazaMedieGenerala(lista));

    char* nume = getNumeStudentCuMedieMaxima(lista);
    if (nume) {
        printf("Studentul cu media maxima: %s\n", nume);
        free(nume);
    }

    stergeStudentDupaID(&lista, 2);

    afisareListaStudenti(lista);

    dezalocareLDStudenti(&lista);

    return 0;
}