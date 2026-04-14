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
	s.medie = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	s.facultate = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.facultate, strlen(aux) + 1, aux);

	s.grupa = *strtok(NULL, sep);

	return s;
}

void afisareStudent(Student s) {
	if (s.id == -1) {
		printf("Student nu exista\n");
		return;
	}

	printf("Id: %d\n", s.id);
	printf("Varsta: %d\n", s.varsta);
	printf("Medie: %.2f\n", s.medie);
	printf("Nume: %s\n", s.nume);
	printf("Facultate: %s\n", s.facultate);
	printf("Grupa: %c\n\n", s.grupa);
}

struct NodSimplu {
	Student info;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

void pushStack(NodSimplu** stack, Student s) {
	NodSimplu* nod = (NodSimplu*)malloc(sizeof(NodSimplu));
	nod->info = s;
	nod->next = *stack;
	*stack = nod;
}

Student popStack(NodSimplu** stack) {
	Student rezultat;
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

NodSimplu* citireStackStudentiDinFisier(const char* numeFisier) {
	NodSimplu* stack = NULL;
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		pushStack(&stack, citireStudentDinFisier(file));
	}

	fclose(file);
	return stack;
}

void dezalocareStivaDeStudenti(NodSimplu** stiva) {
	while (*stiva != NULL) {
		Student s = popStack(stiva);
		free(s.nume);
		free(s.facultate);
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
	Student info;
	struct Nod* prev;
	struct Nod* next;
};
typedef struct Nod Nod;

struct Queue {
	Nod* start;
	Nod* end;
};
typedef struct Queue Queue;

void enqueue(Queue* queue, Student s) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = s;
	nod->next = queue->start;
	nod->prev = NULL;

	if (queue->start != NULL)
		queue->start->prev = nod;
	else
		queue->end = nod;

	queue->start = nod;
}

Student dequeue(Queue* queue) {
	Student s;
	s.id = -1;

	if (queue->start == NULL)
		return s;

	s = queue->end->info;

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
	return s;
}

Queue citireCoadaStudentiDinFisier(const char* numeFisier) {
	Queue queue;
	queue.start = NULL;
	queue.end = NULL;

	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		Student s = citireStudentDinFisier(file);
		enqueue(&queue, s);
	}

	fclose(file);
	return queue;
}

void dezalocareCoadaDeStudenti(Queue* coada) {
	while (coada->start != NULL) {
		Student s = dequeue(coada);
		free(s.nume);
		free(s.facultate);
	}
}

Student getStudentByID(Queue* queue, int id) {
	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	Student rezultat;
	rezultat.id = -1;

	while (queue->start != NULL) {
		Student s = dequeue(queue);

		if (s.id == id) {
			rezultat = s;
			rezultat.nume = (char*)malloc(strlen(s.nume) + 1);
			strcpy(rezultat.nume, s.nume);
			rezultat.facultate = (char*)malloc(strlen(s.facultate) + 1);
			strcpy(rezultat.facultate, s.facultate);
		}

		enqueue(&aux, s);
	}

	queue->start = aux.start;
	queue->end = aux.end;

	return rezultat;
}

float calculeazaMedieTotala(NodSimplu** stack) {
	NodSimplu* aux = NULL;
	float total = 0;

	while (!emptyStack(*stack)) {
		Student s = popStack(stack);
		total += s.medie;
		pushStack(&aux, s);
	}

	while (!emptyStack(aux)) {
		pushStack(stack, popStack(&aux));
	}

	return total;
}

int main() {
	NodSimplu* stiva = citireStackStudentiDinFisier("studenti.txt");
	printf("%d\n", size(stiva));
	printf("%.2f\n", calculeazaMedieTotala(&stiva));

	Queue queue;
	queue.start = NULL;
	queue.end = NULL;

	queue = citireCoadaStudentiDinFisier("studenti.txt");

	Student s = getStudentByID(&queue, 3);
	afisareStudent(s);

	if (s.id != -1) {
		free(s.nume);
		free(s.facultate);
	}

	dezalocareStivaDeStudenti(&stiva);
	dezalocareCoadaDeStudenti(&queue);

	return 0;
}