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
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	s.facultate = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.facultate, strlen(aux) + 1, aux);

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

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Student info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

void inserareInListaP(NodP** graf, Student studentNou) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));

	nou->info = studentNou;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*graf) {
		NodP* aux = *graf;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareInListaS(NodS** listaS, NodP* vecin) {
	NodS* nou = (NodS*)malloc(sizeof(NodS));

	nou->info = vecin;
	nou->next = NULL;

	if (*listaS) {
		NodS* aux = *listaS;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*listaS = nou;
	}
}

NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* aux = listaPrincipala;

	while (aux) {
		if (aux->info.id == id) {
			return aux;
		}

		aux = aux->next;
	}

	return NULL;
}

void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaID(listaPrincipala, idStop);

	if (start && stop) {
		inserareInListaS(&(start->vecini), stop);
		inserareInListaS(&(stop->vecini), start);
	}
}

NodP* citireNoduriStudentiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	NodP* graf = NULL;

	if (f != NULL) {

		while (!feof(f)) {
			Student s = citireStudentDinFisier(f);

			inserareInListaP(&graf, s);
		}

		fclose(f);
	}
	else {
		printf("Eroare la deschiderea fisierului!\n");
	}

	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");

	if (f != NULL) {

		while (!feof(f)) {

			int idStart = 0;
			int idStop = 0;

			if (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
				inserareMuchie(graf, idStart, idStop);
			}
		}

		fclose(f);
	}
	else {
		printf("Eroare la deschiderea fisierului!\n");
	}
}

void dezalocareNoduriGraf(NodP** listaPrincipala) {

	while (*listaPrincipala) {

		NodP* nodCurent = *listaPrincipala;

		NodS* listaVecini = nodCurent->vecini;

		while (listaVecini) {

			NodS* temp = listaVecini;

			listaVecini = listaVecini->next;

			free(temp);
		}

		free(nodCurent->info.nume);
		free(nodCurent->info.facultate);

		*listaPrincipala = (*listaPrincipala)->next;

		free(nodCurent);
	}

	*listaPrincipala = NULL;
}

void afiseazaListaDeVecini(int id, NodP* graf) {

	NodP* temp = cautaNodDupaID(graf, id);

	if (temp != NULL) {

		NodS* lista = temp->vecini;

		printf("Vecinii studentului cu id %d sunt:\n\n", id);

		while (lista) {

			afisareStudent(lista->info->info);

			lista = lista->next;
		}
	}
	else {
		printf("Nu exista studentul cautat!\n");
	}
}

int main() {

	NodP* graf = citireNoduriStudentiDinFisier("studenti.txt");

	citireMuchiiDinFisier("muchii.txt", graf);

	afiseazaListaDeVecini(3, graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}