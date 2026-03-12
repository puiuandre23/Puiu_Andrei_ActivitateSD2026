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

struct Nod {
    Student info;
    struct Nod* next;
};
typedef struct Nod Nod;

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

void afisareListaStudenti(Nod* cap) {
    while (cap != NULL) {
        afisareStudent(cap->info);
        cap = cap->next;
    }
}

void adaugaStudentInLista(Nod** cap, Student studentNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = studentNou;
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

void adaugaLaInceputInLista(Nod** cap, Student studentNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = studentNou;
    nou->next = *cap;
    *cap = nou;
}

Nod* citireListaStudentiDinFisier(const char* numeFisier) {
    Nod* cap = NULL;
    FILE* f = fopen(numeFisier, "r");

    if (f) {
        while (!feof(f)) {
            adaugaStudentInLista(&cap, citireStudentDinFisier(f));
        }
        fclose(f);
    }

    return cap;
}

void dezalocareListaStudenti(Nod** cap) {
    while (*cap) {
        Nod* p = *cap;
        *cap = p->next;

        if (p->info.nume) {
            free(p->info.nume);
        }
        if (p->info.facultate) {
            free(p->info.facultate);
        }

        free(p);
    }
}

float calculeazaMediaGenerala(Nod* cap) {
    float suma = 0;
    int contor = 0;

    while (cap) {
        suma += cap->info.medie;
        contor++;
        cap = cap->next;
    }

    if (contor > 0) {
        return suma / contor;
    }
    return 0;
}

void stergeStudentiDinGrupa(Nod** cap, char grupaCautata) {
    while ((*cap) && (*cap)->info.grupa == grupaCautata) {
        Nod* aux = *cap;
        *cap = aux->next;

        if (aux->info.nume) {
            free(aux->info.nume);
        }
        if (aux->info.facultate) {
            free(aux->info.facultate);
        }

        free(aux);
    }

    if (*cap) {
        Nod* p = *cap;
        while (p) {
            while (p->next && p->next->info.grupa != grupaCautata) {
                p = p->next;
            }

            if (p->next) {
                Nod* aux = p->next;
                p->next = aux->next;

                if (aux->info.nume) {
                    free(aux->info.nume);
                }
                if (aux->info.facultate) {
                    free(aux->info.facultate);
                }

                free(aux);
            }
            else {
                p = NULL;
            }
        }
    }
}

int main() {
    Nod* cap = citireListaStudentiDinFisier("studenti.txt");

    printf("Lista initiala de studenti:\n\n");
    afisareListaStudenti(cap);

    printf("Media generala a studentilor este: %.2f\n\n", calculeazaMediaGenerala(cap));

    printf("Stergere studenti din grupa A:\n\n");
    stergeStudentiDinGrupa(&cap, 'A');
    afisareListaStudenti(cap);

    dezalocareListaStudenti(&cap);

    return 0;
}