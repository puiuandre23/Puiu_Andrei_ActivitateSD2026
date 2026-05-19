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
	unsigned char sector;
};

typedef struct StructuraCasa Casa;

Casa citireCasaDinFisier(FILE* file) {

	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;

	Casa c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);

	c.nrCamere = atoi(strtok(NULL, sep));

	c.pret = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.adresa, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.proprietar = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.proprietar, strlen(aux) + 1, aux);

	c.sector = *strtok(NULL, sep);

	return c;
}

void afisareCasa(Casa casa) {

	printf("Id: %d\n", casa.id);
	printf("Nr camere: %d\n", casa.nrCamere);
	printf("Pret: %.2f\n", casa.pret);
	printf("Adresa: %s\n", casa.adresa);
	printf("Proprietar: %s\n", casa.proprietar);
	printf("Sector: %c\n\n", casa.sector);
}

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Casa info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

void inserareInListaP(NodP** graf, Casa casaNoua) {

	NodP* nou = (NodP*)malloc(sizeof(NodP));

	nou->info = casaNoua;
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

NodP* citireNoduriCaseDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");

	NodP* graf = NULL;

	if (f != NULL) {

		while (!feof(f)) {

			Casa c = citireCasaDinFisier(f);

			inserareInListaP(&graf, c);
		}

		fclose(f);
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

		free(nodCurent->info.adresa);

		free(nodCurent->info.proprietar);

		*listaPrincipala = (*listaPrincipala)->next;

		free(nodCurent);
	}

	*listaPrincipala = NULL;
}

void afiseazaListaDeVecini(int id, NodP* graf) {

	NodP* temp = cautaNodDupaID(graf, id);

	if (temp != NULL) {

		NodS* lista = temp->vecini;

		printf("Vecinii casei cu id %d sunt:\n\n", id);

		while (lista) {

			afisareCasa(lista->info->info);

			lista = lista->next;
		}
	}
}

int main() {

	NodP* graf = citireNoduriCaseDinFisier("Case.txt");

	citireMuchiiDinFisier("muchii.txt", graf);

	afiseazaListaDeVecini(3, graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}