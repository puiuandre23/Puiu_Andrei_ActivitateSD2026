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
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);

	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

void inserareInListaP(NodP** graf, Masina masinaNoua) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));

	nou->info = masinaNoua;
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

NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;

	if (f != NULL) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			inserareInListaP(&graf, m);
		}

		fclose(f);
	}
	else {
		printf("Eroare la deschiderea fisierului %s\n", numeFisier);
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
		printf("Eroare la deschiderea fisierului %s\n", numeFisier);
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

		free(nodCurent->info.model);
		free(nodCurent->info.numeSofer);

		*listaPrincipala = (*listaPrincipala)->next;

		free(nodCurent);
	}

	*listaPrincipala = NULL;
}

void afiseazaListaDeVecini(int id, NodP* graf) {
	NodP* temp = cautaNodDupaID(graf, id);

	if (temp != NULL) {
		NodS* lista = temp->vecini;

		printf("Vecinii nodului cu id %d sunt:\n", id);

		while (lista) {
			afisareMasina(lista->info->info);
			lista = lista->next;
		}
	}
	else {
		printf("Nu exista nod cu id-ul %d\n", id);
	}
}

int main() {
	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");

	citireMuchiiDinFisier("muchii.txt", graf);

	afiseazaListaDeVecini(3, graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}