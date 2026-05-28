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
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

typedef struct Nod Nod;
typedef struct NodS NodS;

struct Nod {
	Masina info;
	Nod* next;
	NodS* vecini;
};

struct NodS {
	Nod* info;
	NodS* next;
};

void inserareInListaPrincipala(Nod** graf, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->vecini = NULL;
	nou->info = m;

	if (*graf == NULL) {
		*graf = nou;
	}
	else {
		Nod* p = *graf;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = nou;
	}
}

void inserareListaSecundara(NodS** cap, Nod* vecin) {
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->next = NULL;
	nou->info = vecin;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodS* p = *cap;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = nou;
	}
}

Nod* cautaNodDupaID(Nod* graf, int id) {
	while (graf && graf->info.id != id) {
		graf = graf->next;
	}
	return graf;
}

void inserareMuchie(Nod* graf, int idStart, int idStop) {
	Nod* nodStart = cautaNodDupaID(graf, idStart);
	Nod* nodStop = cautaNodDupaID(graf, idStop);

	if (nodStart && nodStop) {
		inserareListaSecundara(&nodStart->vecini, nodStop);
		inserareListaSecundara(&nodStop->vecini, nodStart);
	}
}

Nod* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* graf = NULL;

	if (f != NULL) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			inserareInListaPrincipala(&graf, m);
		}
		fclose(f);
	}
	else {
		printf("Eroare fisier noduri\n");
	}

	return graf;
}

void citireMuchiiDinFisier(Nod* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	if (f != NULL) {
		int start, stop;

		while (fscanf(f, "%d %d", &start, &stop) == 2) {
			inserareMuchie(graf, start, stop);
		}

		fclose(f);
	}
	else {
		printf("Eroare fisier muchii\n");
	}
}

void afisareVecini(Nod* graf, int id) {
	Nod* nod = cautaNodDupaID(graf, id);

	if (nod != NULL) {
		printf("Vecinii nodului cu id %d:\n", id);

		NodS* vecin = nod->vecini;
		while (vecin != NULL) {
			afisareMasina(vecin->info->info);
			vecin = vecin->next;
		}
	}
}

int calculeazaNrNoduriGraf(Nod* listaPrincipala) {
	int nr = 0;

	while (listaPrincipala != NULL) {
		nr++;
		listaPrincipala = listaPrincipala->next;
	}

	return nr;
}

int pozitieNod(Nod* graf, Nod* nodCautat) {
	int poz = 0;

	while (graf != NULL) {
		if (graf == nodCautat) {
			return poz;
		}

		poz++;
		graf = graf->next;
	}

	return -1;
}

typedef struct NodStiva {
	Nod* info;
	struct NodStiva* next;
} NodStiva;

void push(NodStiva** stiva, Nod* nod) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->info = nod;
	nou->next = *stiva;
	*stiva = nou;
}

Nod* pop(NodStiva** stiva) {
	if (*stiva != NULL) {
		NodStiva* temp = *stiva;
		Nod* nod = temp->info;
		*stiva = (*stiva)->next;
		free(temp);
		return nod;
	}

	return NULL;
}

void afisareGrafInAdancime(Nod* listaPrincipala, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(listaPrincipala);
	int* vizitate = (int*)calloc(nrNoduri, sizeof(int));

	Nod* nodStart = cautaNodDupaID(listaPrincipala, idPlecare);

	if (nodStart != NULL) {
		NodStiva* stiva = NULL;

		int pozStart = pozitieNod(listaPrincipala, nodStart);
		vizitate[pozStart] = 1;
		push(&stiva, nodStart);

		printf("Parcurgere in adancime:\n");

		while (stiva != NULL) {
			Nod* nodCurent = pop(&stiva);
			afisareMasina(nodCurent->info);

			NodS* vecin = nodCurent->vecini;

			while (vecin != NULL) {
				int poz = pozitieNod(listaPrincipala, vecin->info);

				if (vizitate[poz] == 0) {
					vizitate[poz] = 1;
					push(&stiva, vecin->info);
				}

				vecin = vecin->next;
			}
		}
	}

	free(vizitate);
}

typedef struct NodCoada {
	Nod* info;
	struct NodCoada* next;
} NodCoada;

void enqueue(NodCoada** prim, NodCoada** ultim, Nod* nod) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->info = nod;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

Nod* dequeue(NodCoada** prim, NodCoada** ultim) {
	if (*prim != NULL) {
		NodCoada* temp = *prim;
		Nod* nod = temp->info;

		*prim = (*prim)->next;

		if (*prim == NULL) {
			*ultim = NULL;
		}

		free(temp);
		return nod;
	}

	return NULL;
}

void afisareGrafInLatime(Nod* listaPrincipala, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(listaPrincipala);
	int* vizitate = (int*)calloc(nrNoduri, sizeof(int));

	Nod* nodStart = cautaNodDupaID(listaPrincipala, idPlecare);

	if (nodStart != NULL) {
		NodCoada* prim = NULL;
		NodCoada* ultim = NULL;

		int pozStart = pozitieNod(listaPrincipala, nodStart);
		vizitate[pozStart] = 1;
		enqueue(&prim, &ultim, nodStart);

		printf("Parcurgere in latime:\n");

		while (prim != NULL) {
			Nod* nodCurent = dequeue(&prim, &ultim);
			afisareMasina(nodCurent->info);

			NodS* vecin = nodCurent->vecini;

			while (vecin != NULL) {
				int poz = pozitieNod(listaPrincipala, vecin->info);

				if (vizitate[poz] == 0) {
					vizitate[poz] = 1;
					enqueue(&prim, &ultim, vecin->info);
				}

				vecin = vecin->next;
			}
		}
	}

	free(vizitate);
}

void dezalocareNoduriGraf(Nod** listaPrincipala) {
	while (*listaPrincipala != NULL) {
		Nod* nodCurent = *listaPrincipala;

		NodS* vecini = nodCurent->vecini;

		while (vecini != NULL) {
			NodS* temp = vecini;
			vecini = vecini->next;
			free(temp);
		}

		free(nodCurent->info.model);
		free(nodCurent->info.numeSofer);

		*listaPrincipala = (*listaPrincipala)->next;
		free(nodCurent);
	}
}

int main() {
	Nod* graf = citireNoduriMasiniDinFisier("masini.txt");

	citireMuchiiDinFisier(graf, "muchii.txt");

	printf("Numar noduri graf: %d\n\n", calculeazaNrNoduriGraf(graf));

	afisareVecini(graf, 3);

	afisareGrafInAdancime(graf, 1);

	afisareGrafInLatime(graf, 1);

	dezalocareNoduriGraf(&graf);

	return 0;
}