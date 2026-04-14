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
	unsigned char categorie;
};
typedef struct StructuraCasa Casa;

typedef struct Nod Nod;
struct Nod
{
	Casa info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Casa citireCasaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Casa c1;
	aux = strtok(buffer, sep);
	c1.id = atoi(aux);
	c1.nrCamere = atoi(strtok(NULL, sep));
	c1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	c1.adresa = malloc(strlen(aux) + 1);
	strcpy_s(c1.adresa, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c1.proprietar = malloc(strlen(aux) + 1);
	strcpy_s(c1.proprietar, strlen(aux) + 1, aux);

	c1.categorie = *strtok(NULL, sep);
	return c1;
}

void afisareCasa(Casa casa) {
	printf("Id: %d\n", casa.id);
	printf("Nr. camere : %d\n", casa.nrCamere);
	printf("Pret: %.2f\n", casa.pret);
	printf("Adresa: %s\n", casa.adresa);
	printf("Proprietar: %s\n", casa.proprietar);
	printf("Categorie: %c\n\n", casa.categorie);
}

void afisareListaCase(Nod* cap) {
	while (cap)
	{
		afisareCasa(cap->info);
		cap = cap->next;
	}
}

void adaugaCasaInLista(Nod** lista, Casa casaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = casaNoua;
	nodNou->next = NULL;
	if (*lista)
	{
		Nod* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}
	else
	{
		(*lista) = nodNou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	id = id * 23 + 123;
	return id % dimensiune;
}

void inserareCasaInTabela(HashTable hash, Casa casa) {
	int poz = calculeazaHash(casa.id, hash.dim);
	if (hash.vector[poz])
	{
		adaugaCasaInLista(&(hash.vector[poz]), casa);
	}
	else
	{
		adaugaCasaInLista(&(hash.vector[poz]), casa);
	}
}

HashTable citireCaseDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	HashTable tabela = initializareHashTable(3);
	if (!f)
	{
		printf("nu exista!\n");
		return tabela;
	}
	while (!feof(f))
	{
		inserareCasaInTabela(tabela, citireCasaDinFisier(f));
	}
	fclose(f);
	return tabela;
}

void afisareTabelaDeCase(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("Cluster: %d\n", i + 1);
			afisareListaCase(ht.vector[i]);
			printf("======================\n");
		}
	}
}

void dezalocareTabelaDeCase(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++)
	{
		Nod* aux = ht->vector[i];
		while (aux)
		{
			Nod* temp = aux;
			aux = aux->next;
			free(temp->info.adresa);
			free(temp->info.proprietar);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	float* preturi_medii = malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			float suma = 0;
			int nrCase = 0;
			Nod* aux = ht.vector[i];
			while (aux)
			{
				suma += aux->info.pret;
				nrCase++;
				aux = aux->next;
			}
			preturi_medii[k++] = suma / nrCase;
		}
	}
	return preturi_medii;
}

Casa getCasaDupaID(HashTable ht, int id) {
	Casa c;
	c.id = -1;
	int hash = calculeazaHash(id, ht.dim);
	if (hash >= 0 && hash < ht.dim)
	{
		Nod* aux = ht.vector[hash];
		while (aux)
		{
			if (aux->info.id == id)
			{
				c = aux->info;
				c.proprietar = malloc(sizeof(char) * strlen(aux->info.proprietar) + 1);
				strcpy(c.proprietar, aux->info.proprietar);
				c.adresa = malloc(sizeof(char) * strlen(aux->info.adresa) + 1);
				strcpy(c.adresa, aux->info.adresa);
				break;
			}
			aux = aux->next;
		}
	}

	return c;
}

int main() {

	HashTable tabela = citireCaseDinFisier("Case.txt");
	afisareTabelaDeCase(tabela);
	Casa c = getCasaDupaID(tabela, 5);
	afisareCasa(c);
	int nrClustere;
	float* vectorPreturi = calculeazaPreturiMediiPerClustere(tabela, &nrClustere);
	for (int i = 0; i < nrClustere; i++)
	{
		printf("Pentru clusterul cu index %d, pretul mediu este:%.2f\n", i, vectorPreturi[i]);
	}

	free(c.adresa);
	free(c.proprietar);
	free(vectorPreturi);
	dezalocareTabelaDeCase(&tabela);

	return 0;
}