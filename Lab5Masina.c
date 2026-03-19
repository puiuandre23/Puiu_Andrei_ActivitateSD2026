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

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Masina m1;

	if (fgets(buffer, 100, file) == NULL) {
		m1.id = -1;
		m1.nrUsi = 0;
		m1.pret = 0;
		m1.model = NULL;
		m1.numeSofer = NULL;
		m1.serie = 0;
		return m1;
	}

	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

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
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->ultim;

	if (lista->ultim != NULL) {
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}

	lista->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = lista->prim;
	nou->prev = NULL;

	if (lista->prim != NULL) {
		lista->prim->prev = nou;
	}
	else {
		lista->ultim = nou;
	}

	lista->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	if (f != NULL) {
		while (1) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id == -1) {
				break;
			}
			adaugaMasinaInLista(&lista, m);
		}
		fclose(f);
	}

	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->prim;
	while (p) {
		Nod* aux = p;
		p = p->next;

		if (aux->info.model != NULL) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer != NULL) {
			free(aux->info.numeSofer);
		}
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

	if (contor == 0) {
		return 0;
	}

	return suma / contor;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	Nod* p = lista->prim;

	while (p != NULL && p->info.id != id) {
		p = p->next;
	}

	if (p == NULL) {
		return;
	}

	if (p->prev != NULL) {
		p->prev->next = p->next;
	}
	else {
		lista->prim = p->next;
	}

	if (p->next != NULL) {
		p->next->prev = p->prev;
	}
	else {
		lista->ultim = p->prev;
	}

	if (p->info.model != NULL) {
		free(p->info.model);
	}
	if (p->info.numeSofer != NULL) {
		free(p->info.numeSofer);
	}

	free(p);
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.prim == NULL) {
		return NULL;
	}

	Nod* max = lista.prim;
	Nod* p = lista.prim->next;

	while (p) {
		if (p->info.pret > max->info.pret) {
			max = p;
		}
		p = p->next;
	}

	char* nume = (char*)malloc(strlen(max->info.numeSofer) + 1);
	strcpy_s(nume, strlen(max->info.numeSofer) + 1, max->info.numeSofer);
	return nume;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");

	printf("Afisare lista:\n");
	afisareListaMasini(lista);

	printf("Pretul mediu este: %.2f\n", calculeazaPretMediu(lista));

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	if (numeSofer != NULL) {
		printf("Soferul masinii celei mai scumpe este: %s\n", numeSofer);
		free(numeSofer);
	}

	stergeMasinaDupaID(&lista, 2);
	printf("\nDupa stergerea masinii cu id = 2:\n");
	afisareListaMasini(lista);

	dezalocareLDMasini(&lista);

	return 0;
}


