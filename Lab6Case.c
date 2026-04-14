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

Casa citireCasaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Casa c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	c.nrCamere = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.adresa, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.proprietar = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.proprietar, strlen(aux) + 1, aux);

	c.categorie = *strtok(NULL, sep);

	return c;
}

void afisareCasa(Casa casa) {
	if (casa.id == -1) {
		printf("Casa nu exista\n");
		return;
	}

	printf("Id: %d\n", casa.id);
	printf("Nr camere: %d\n", casa.nrCamere);
	printf("Pret: %.2f\n", casa.pret);
	printf("Adresa: %s\n", casa.adresa);
	printf("Proprietar: %s\n", casa.proprietar);
	printf("Categorie: %c\n\n", casa.categorie);
}

struct NodSimplu {
	Casa info;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

void pushStack(NodSimplu** stack, Casa casa) {
	NodSimplu* nod = (NodSimplu*)malloc(sizeof(NodSimplu));
	nod->info = casa;
	nod->next = *stack;
	*stack = nod;
}

Casa popStack(NodSimplu** stack) {
	Casa rezultat;
	rezultat.id = -1;

	if (*stack != NULL) {
		NodSimplu* aux = *stack;
		*stack = aux->next;
		rezultat = aux->info;
		free(aux);
	}
	return rezultat;
}

char emptyStack(NodSimplu* stack) {
	return stack == NULL;
}

NodSimplu* citireStackCaseDinFisier(const char* numeFisier) {
	NodSimplu* stack = NULL;
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		pushStack(&stack, citireCasaDinFisier(file));
	}

	fclose(file);
	return stack;
}

void dezalocareStiva(NodSimplu** stiva) {
	while (*stiva != NULL) {
		Casa c = popStack(stiva);
		free(c.adresa);
		free(c.proprietar);
	}
}

int size(NodSimplu* stiva) {
	int nr = 0;
	while (stiva != NULL) {
		nr++;
		stiva = stiva->next;
	}
	return nr;
}

struct Nod {
	Casa info;
	struct Nod* prev;
	struct Nod* next;
};
typedef struct Nod Nod;

struct Queue {
	Nod* start;
	Nod* end;
};
typedef struct Queue Queue;

void enqueue(Queue* queue, Casa casa) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = casa;
	nod->next = queue->start;
	nod->prev = NULL;

	if (queue->start != NULL)
		queue->start->prev = nod;
	else
		queue->end = nod;

	queue->start = nod;
}

Casa dequeue(Queue* queue) {
	Casa c;
	c.id = -1;

	if (queue->start == NULL)
		return c;

	c = queue->end->info;

	if (queue->start == queue->end) {
		free(queue->start);
		queue->start = NULL;
		queue->end = NULL;
	}
	else {
		queue->end = queue->end->prev;
		free(queue->end->next);
		queue->end->next = NULL;
	}
	return c;
}

Queue citireCoadaCaseDinFisier(const char* numeFisier) {
	Queue queue;
	queue.start = NULL;
	queue.end = NULL;

	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		Casa c = citireCasaDinFisier(file);
		enqueue(&queue, c);
	}

	fclose(file);
	return queue;
}

void dezalocareCoada(Queue* coada) {
	while (coada->start != NULL) {
		Casa c = dequeue(coada);
		free(c.adresa);
		free(c.proprietar);
	}
}

Casa getCasaByID(Queue* queue, int id) {
	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	Casa rezultat;
	rezultat.id = -1;

	while (queue->start != NULL) {
		Casa c = dequeue(queue);

		if (c.id == id) {
			rezultat = c;
			rezultat.adresa = (char*)malloc(strlen(c.adresa) + 1);
			strcpy(rezultat.adresa, c.adresa);
			rezultat.proprietar = (char*)malloc(strlen(c.proprietar) + 1);
			strcpy(rezultat.proprietar, c.proprietar);
		}

		enqueue(&aux, c);
	}

	queue->start = aux.start;
	queue->end = aux.end;

	return rezultat;
}


float calculeazaPretTotal(NodSimplu** stack) {
	NodSimplu* aux = NULL;
	float total = 0;

	while (!emptyStack(*stack)) {
		Casa c = popStack(stack);
		total += c.pret;
		pushStack(&aux, c);
	}

	while (!emptyStack(aux)) {
		pushStack(stack, popStack(&aux));
	}

	return total;
}

int main() {
	Queue queue;
	queue.start = NULL;
	queue.end = NULL;

	queue = citireCoadaCaseDinFisier("Case.txt");

	Casa c = getCasaByID(&queue, 3);
	afisareCasa(c);

	if (c.id != -1) {
		free(c.adresa);
		free(c.proprietar);
	}

	dezalocareCoada(&queue);

	return 0;
}