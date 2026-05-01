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

struct Heap {
	Masina* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Masina m1;

	fgets(buffer, 100, file);

	aux = strtok(buffer, sep);
	m1.id = atoi(aux);

	aux = strtok(NULL, sep);
	m1.nrUsi = atoi(aux);

	aux = strtok(NULL, sep);
	m1.pret = (float)atof(aux);

	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	aux = strtok(NULL, sep);
	m1.serie = aux[0];

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;
	int pozitieMax = pozitieNod;

	if (pozitieStanga < heap.nrElemente &&
		heap.vector[pozitieStanga].id > heap.vector[pozitieMax].id) {
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap.nrElemente &&
		heap.vector[pozitieDreapta].id > heap.vector[pozitieMax].id) {
		pozitieMax = pozitieDreapta;
	}

	if (pozitieMax != pozitieNod) {
		Masina aux = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		filtreazaHeap(heap, pozitieMax);
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	if (f != NULL) {
		while (!feof(f)) {
			if (heap.nrElemente < heap.lungime) {
				heap.vector[heap.nrElemente] = citireMasinaDinFisier(f);
				heap.nrElemente++;
			}
		}

		fclose(f);

		heap.lungime = heap.nrElemente;

		for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
			filtreazaHeap(heap, i);
		}
	}
	else {
		printf("Fisierul nu a putut fi deschis.\n");
	}

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(void* heap) {
	Heap* h = (Heap*)heap;

	if (h->nrElemente > 0) {
		Masina aux = h->vector[0];

		h->vector[0] = h->vector[h->nrElemente - 1];
		h->vector[h->nrElemente - 1] = aux;

		h->nrElemente--;

		filtreazaHeap(*h, 0);

		return aux;
	}

	Masina m;
	m.id = -1;
	m.nrUsi = 0;
	m.pret = 0;
	m.model = NULL;
	m.numeSofer = NULL;
	m.serie = '-';
	return m;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");

	printf("Elementele din heap sunt:\n");
	afisareHeap(heap);

	printf("\nMasina extrasa:\n");
	Masina m = extrageMasina(&heap);
	afisareMasina(m);

	printf("\nElementele vizibile ramase in heap sunt:\n");
	afisareHeap(heap);

	printf("\nElementele ascunse din heap sunt:\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}