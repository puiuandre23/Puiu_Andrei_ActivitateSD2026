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
struct Nod
{
	Student info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Student s1;
	aux = strtok(buffer, sep);
	s1.id = atoi(aux);
	s1.varsta = atoi(strtok(NULL, sep));
	s1.medie = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	s1.nume = malloc(strlen(aux) + 1);
	strcpy_s(s1.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	s1.facultate = malloc(strlen(aux) + 1);
	strcpy_s(s1.facultate, strlen(aux) + 1, aux);

	s1.grupa = *strtok(NULL, sep);
	return s1;
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
	while (cap)
	{
		afisareStudent(cap->info);
		cap = cap->next;
	}
}

void adaugaStudentInLista(Nod** lista, Student studentNou) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = studentNou;
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

void inserareStudentInTabela(HashTable hash, Student student) {
	int poz = calculeazaHash(student.id, hash.dim);
	if (hash.vector[poz])
	{
		adaugaStudentInLista(&(hash.vector[poz]), student);
	}
	else
	{
		adaugaStudentInLista(&(hash.vector[poz]), student);
	}
}

HashTable citireStudentiDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	HashTable tabela = initializareHashTable(3);
	if (!f)
	{
		printf("nu exista!\n");
		return tabela;
	}
	while (!feof(f))
	{
		inserareStudentInTabela(tabela, citireStudentDinFisier(f));
	}
	fclose(f);
	return tabela;
}

void afisareTabelaDeStudenti(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("Cluster: %d\n", i + 1);
			afisareListaStudenti(ht.vector[i]);
			printf("======================\n");
		}
	}
}

void dezalocareTabelaDeStudenti(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++)
	{
		Nod* aux = ht->vector[i];
		while (aux)
		{
			Nod* temp = aux;
			aux = aux->next;
			free(temp->info.nume);
			free(temp->info.facultate);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	float* medii = malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			float suma = 0;
			int nrStudenti = 0;
			Nod* aux = ht.vector[i];
			while (aux)
			{
				suma += aux->info.medie;
				nrStudenti++;
				aux = aux->next;
			}
			medii[k++] = suma / nrStudenti;
		}
	}
	return medii;
}

Student getStudentDupaID(HashTable ht, int id) {
	Student s;
	s.id = -1;
	int hash = calculeazaHash(id, ht.dim);
	if (hash >= 0 && hash < ht.dim)
	{
		Nod* aux = ht.vector[hash];
		while (aux)
		{
			if (aux->info.id == id)
			{
				s = aux->info;
				s.nume = malloc(sizeof(char) * strlen(aux->info.nume) + 1);
				strcpy(s.nume, aux->info.nume);
				s.facultate = malloc(sizeof(char) * strlen(aux->info.facultate) + 1);
				strcpy(s.facultate, aux->info.facultate);
				break;
			}
			aux = aux->next;
		}
	}

	return s;
}

int main() {

	HashTable tabela = citireStudentiDinFisier("studenti.txt");
	afisareTabelaDeStudenti(tabela);
	Student s = getStudentDupaID(tabela, 5);
	afisareStudent(s);
	int nrClustere;
	float* vectorMedii = calculeazaMediiPerClustere(tabela, &nrClustere);
	for (int i = 0; i < nrClustere; i++)
	{
		printf("Pentru clusterul cu index %d, media este:%.2f\n", i, vectorMedii[i]);
	}

	free(s.nume);
	free(s.facultate);
	free(vectorMedii);
	dezalocareTabelaDeStudenti(&tabela);

	return 0;
}