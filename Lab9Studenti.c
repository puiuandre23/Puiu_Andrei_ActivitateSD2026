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
    struct Nod* stanga;
    struct Nod* dreapta;
};
typedef struct Nod Nod;

Student citireStudentDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Student s;

    fgets(buffer, 100, file);

    aux = strtok(buffer, sep);
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

void afisareStudent(Student student) {
    printf("Id: %d\n", student.id);
    printf("Varsta: %d\n", student.varsta);
    printf("Medie: %.2f\n", student.medie);
    printf("Nume: %s\n", student.nume);
    printf("Facultate: %s\n", student.facultate);
    printf("Grupa: %c\n\n", student.grupa);
}

void adaugaStudentInArbore(Nod** radacina, Student studentNou) {
    if ((*radacina) == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = studentNou;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        (*radacina) = nou;
    }
    else {
        if (studentNou.id < (*radacina)->info.id) {
            adaugaStudentInArbore(&((*radacina)->stanga), studentNou);
        }
        else {
            adaugaStudentInArbore(&((*radacina)->dreapta), studentNou);
        }
    }
}

Nod* citireArboreDeStudentiDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* radacina = NULL;

    if (f != NULL) {
        while (!feof(f)) {
            Student s = citireStudentDinFisier(f);
            adaugaStudentInArbore(&radacina, s);
        }
        fclose(f);
    }
    else {
        printf("Fisierul nu a putut fi deschis.\n");
    }

    return radacina;
}

void afisareStudentiDinArbore(Nod* radacina) {
    if (radacina != NULL) {
        afisareStudentiDinArbore(radacina->stanga);
        afisareStudent(radacina->info);
        afisareStudentiDinArbore(radacina->dreapta);
    }
}

void dezalocareArboreDeStudenti(Nod** radacina) {
    if ((*radacina) != NULL) {
        dezalocareArboreDeStudenti(&((*radacina)->stanga));
        dezalocareArboreDeStudenti(&((*radacina)->dreapta));

        free((*radacina)->info.nume);
        free((*radacina)->info.facultate);
        free(*radacina);
        (*radacina) = NULL;
    }
}

Student getStudentByID(Nod* radacina, int id) {
    if (radacina != NULL) {
        if (radacina->info.id == id) {
            Student s;
            s.id = radacina->info.id;
            s.varsta = radacina->info.varsta;
            s.medie = radacina->info.medie;
            s.grupa = radacina->info.grupa;

            s.nume = (char*)malloc(strlen(radacina->info.nume) + 1);
            strcpy(s.nume, radacina->info.nume);

            s.facultate = (char*)malloc(strlen(radacina->info.facultate) + 1);
            strcpy(s.facultate, radacina->info.facultate);

            return s;
        }
        else if (id < radacina->info.id) {
            return getStudentByID(radacina->stanga, id);
        }
        else {
            return getStudentByID(radacina->dreapta, id);
        }
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

int determinaNumarNoduri(Nod* radacina) {
    if (radacina != NULL) {
        return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
    }
    return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
    if (radacina != NULL) {
        int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
        int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);

        if (inaltimeStanga > inaltimeDreapta) {
            return 1 + inaltimeStanga;
        }
        else {
            return 1 + inaltimeDreapta;
        }
    }
    return 0;
}

float calculeazaMedieTotala(Nod* radacina) {
    if (radacina != NULL) {
        return radacina->info.medie +
            calculeazaMedieTotala(radacina->stanga) +
            calculeazaMedieTotala(radacina->dreapta);
    }
    return 0;
}

float calculeazaMediaStudentilorDinFacultate(Nod* radacina, const char* facultate) {
    if (radacina != NULL) {
        float suma = 0;

        if (strcmp(radacina->info.facultate, facultate) == 0) {
            suma += radacina->info.medie;
        }

        suma += calculeazaMediaStudentilorDinFacultate(radacina->stanga, facultate);
        suma += calculeazaMediaStudentilorDinFacultate(radacina->dreapta, facultate);

        return suma;
    }
    return 0;
}

void afisarePreordine(Nod* radacina) {
    if (radacina != NULL) {
        afisareStudent(radacina->info);
        afisarePreordine(radacina->stanga);
        afisarePreordine(radacina->dreapta);
    }
}

void afisareInordine(Nod* radacina) {
    if (radacina != NULL) {
        afisareInordine(radacina->stanga);
        afisareStudent(radacina->info);
        afisareInordine(radacina->dreapta);
    }
}

void afisarePostordine(Nod* radacina) {
    if (radacina != NULL) {
        afisarePostordine(radacina->stanga);
        afisarePostordine(radacina->dreapta);
        afisareStudent(radacina->info);
    }
}

int main() {
    Nod* radacina = citireArboreDeStudentiDinFisier("studenti.txt");

    printf("Studentii din arbore sunt:\n");
    afisareStudentiDinArbore(radacina);

    printf("Numar noduri: %d\n", determinaNumarNoduri(radacina));
    printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
    printf("Suma mediilor: %.2f\n", calculeazaMedieTotala(radacina));
    printf("Suma mediilor studentilor de la CSIE: %.2f\n\n", calculeazaMediaStudentilorDinFacultate(radacina, "CSIE"));

    Student s = getStudentByID(radacina, 3);
    if (s.id != -1) {
        printf("Studentul cautat este:\n");
        afisareStudent(s);
        free(s.nume);
        free(s.facultate);
    }
    else {
        printf("Studentul nu a fost gasit.\n");
    }

    printf("\n--- PREORDINE ---\n");
    afisarePreordine(radacina);

    printf("\n--- INORDINE ---\n");
    afisareInordine(radacina);

    printf("\n--- POSTORDINE ---\n");
    afisarePostordine(radacina);
    dezalocareArboreDeStudenti(&radacina);

    return 0;
}
