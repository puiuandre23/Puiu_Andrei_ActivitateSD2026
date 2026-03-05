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

void afisareStudent(Student student) {
	printf("ID:%d\n", student.id);
	printf("Varsta:%d\n", student.varsta);
	printf("Medie:%.2f\n", student.medie);
	printf("Nume:%s\n", student.nume);
	printf("Facultate:%s\n", student.facultate);
	printf("Grupa:%c\n\n", student.grupa);
}

void afisareVectorStudenti(Student* studenti, int nrStudenti) {
	for (int i = 0; i < nrStudenti; i++) {
		afisareStudent(studenti[i]);
	}
}

void adaugaStudentInVector(Student** studenti, int* nrStudenti, Student studentNou) {

	Student* aux = (Student*)malloc(sizeof(Student) * ((*nrStudenti) + 1));

	for (int i = 0; i < *nrStudenti; i++) {
		aux[i] = (*studenti)[i];
	}

	aux[*nrStudenti] = studentNou;

	free(*studenti);

	*studenti = aux;

	(*nrStudenti)++;
}

Student citireStudentFisier(FILE* file) {

	Student s;

	char buffer[100];
	char sep[4] = ",;\n";

	fgets(buffer, 100, file);

	s.id = atoi(strtok(buffer, sep));
	s.varsta = atoi(strtok(NULL, sep));
	s.medie = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	s.facultate = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.facultate, strlen(aux) + 1, aux);

	s.grupa = strtok(NULL, sep)[0];

	return s;
}

Student* citireVectorStudentiFisier(const char* numeFisier, int* nrStudentiCititi) {

	FILE* file = fopen(numeFisier, "r");

	Student* studenti = NULL;

	(*nrStudentiCititi) = 0;

	while (!feof(file)) {
		adaugaStudentInVector(&studenti, nrStudentiCititi, citireStudentFisier(file));
	}

	fclose(file);

	return studenti;
}

void dezalocareVectorStudenti(Student** vector, int* nrStudenti) {

	for (int i = 0; i < *nrStudenti; i++) {

		if ((*vector)[i].nume != NULL) {
			free((*vector)[i].nume);
		}

		if ((*vector)[i].facultate != NULL) {
			free((*vector)[i].facultate);
		}
	}

	free(*vector);

	*vector = NULL;

	*nrStudenti = 0;
}

int main() {

	int nrStudenti = 0;

	Student* studenti = citireVectorStudentiFisier("studenti.txt", &nrStudenti);

	afisareVectorStudenti(studenti, nrStudenti);

	dezalocareVectorStudenti(&studenti, &nrStudenti);

	return 0;
}