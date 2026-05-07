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

typedef struct NodArbore NodArbore;
struct NodArbore {
	Student info;
	NodArbore* right;
	NodArbore* left;
};

Student citireStudentDinFisier(FILE* file) {
	char buffer[150];
	char sep[3] = ",\n";
	fgets(buffer, 150, file);
	char* aux;
	Student s;

	aux = strtok(buffer, sep);
	s.id = atoi(aux);

	aux = strtok(NULL, sep);
	s.varsta = atoi(aux);

	aux = strtok(NULL, sep);
	s.medie = (float)atof(aux);

	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(s.nume, aux);

	aux = strtok(NULL, sep);
	s.facultate = (char*)malloc(strlen(aux) + 1);
	strcpy(s.facultate, aux);

	aux = strtok(NULL, sep);
	s.grupa = aux[0];

	return s;
}

void afisareStudent(Student student) {
	printf("Id: %d\n", student.id);
	printf("Varsta: %d\n", student.varsta);
	printf("Medie: %.2f\n", student.medie);
	printf("Nume: %s\n", student.nume);
	printf("Facultate: %s\n", student.facultate);
	printf("Grupa: %c\n\n", student.grupa);
}

int calculeazaInaltimeArbore(NodArbore* root) {
	if (root == NULL) {
		return 0;
	}

	int dr = calculeazaInaltimeArbore(root->right);
	int st = calculeazaInaltimeArbore(root->left);

	return 1 + (dr > st ? dr : st);
}

void rotireStanga(NodArbore** root) {
	NodArbore* aux = (*root)->right;
	(*root)->right = aux->left;
	aux->left = (*root);
	(*root) = aux;
}

void rotireDreapta(NodArbore** root) {
	NodArbore* aux = (*root)->left;
	(*root)->left = aux->right;
	aux->right = (*root);
	(*root) = aux;
}

int getGradEchilibru(NodArbore* root) {
	if (root == NULL) {
		return 0;
	}

	int st = calculeazaInaltimeArbore(root->left);
	int dr = calculeazaInaltimeArbore(root->right);

	return st - dr;
}

void adaugaStudentInArboreEchilibrat(NodArbore** root, Student studentNou) {
	if ((*root) != NULL) {
		if (studentNou.id > (*root)->info.id) {
			adaugaStudentInArboreEchilibrat(&(*root)->right, studentNou);
		}
		else {
			adaugaStudentInArboreEchilibrat(&(*root)->left, studentNou);
		}

		int gradEchilibru = getGradEchilibru(*root);

		if (gradEchilibru == -2) {
			if (getGradEchilibru((*root)->right) <= 0) {
				rotireStanga(root);
			}
			else {
				rotireDreapta(&(*root)->right);
				rotireStanga(root);
			}
		}

		if (gradEchilibru == 2) {
			if (getGradEchilibru((*root)->left) >= 0) {
				rotireDreapta(root);
			}
			else {
				rotireStanga(&(*root)->left);
				rotireDreapta(root);
			}
		}
	}
	else {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->left = NULL;
		nou->right = NULL;
		nou->info = studentNou;
		(*root) = nou;
	}
}

NodArbore* citireArboreDeStudentiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	if (file != NULL) {
		while (!feof(file)) {
			Student student = citireStudentDinFisier(file);
			adaugaStudentInArboreEchilibrat(&root, student);
		}
		fclose(file);
	}

	return root;
}

void afisareStudentiDinArborePreordine(NodArbore* root) {
	if (root) {
		afisareStudent(root->info);
		afisareStudentiDinArborePreordine(root->left);
		afisareStudentiDinArborePreordine(root->right);
	}
}

void afisareStudentiDinArboreInordine(NodArbore* root) {
	if (root) {
		afisareStudentiDinArboreInordine(root->left);
		afisareStudent(root->info);
		afisareStudentiDinArboreInordine(root->right);
	}
}

void afisareStudentiDinArborePostordine(NodArbore* root) {
	if (root) {
		afisareStudentiDinArborePostordine(root->left);
		afisareStudentiDinArborePostordine(root->right);
		afisareStudent(root->info);
	}
}

void afisareStudentiDinArbore(NodArbore* root) {
	printf("\nPREORDINE:\n");
	afisareStudentiDinArborePreordine(root);

	printf("\nINORDINE:\n");
	afisareStudentiDinArboreInordine(root);

	printf("\nPOSTORDINE:\n");
	afisareStudentiDinArborePostordine(root);
}

void dezalocareArboreDeStudenti(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeStudenti(&(*root)->left);
		dezalocareArboreDeStudenti(&(*root)->right);

		free((*root)->info.nume);
		free((*root)->info.facultate);
		free(*root);
		*root = NULL;
	}
}

Student getStudentByID(NodArbore* root, int id) {
	if (root != NULL) {
		if (root->info.id == id) {
			Student student;
			student.id = root->info.id;
			student.varsta = root->info.varsta;
			student.medie = root->info.medie;
			student.grupa = root->info.grupa;

			student.nume = (char*)malloc(strlen(root->info.nume) + 1);
			strcpy(student.nume, root->info.nume);

			student.facultate = (char*)malloc(strlen(root->info.facultate) + 1);
			strcpy(student.facultate, root->info.facultate);

			return student;
		}
		else if (id < root->info.id) {
			return getStudentByID(root->left, id);
		}
		else {
			return getStudentByID(root->right, id);
		}
	}
	else {
		Student student;
		student.id = -1;
		student.varsta = 0;
		student.medie = 0;
		student.nume = NULL;
		student.facultate = NULL;
		student.grupa = '-';
		return student;
	}
}

int determinaNumarNoduri(NodArbore* root) {
	if (root) {
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	}
	else {
		return 0;
	}
}

float calculeazaMedieTotala(NodArbore* root) {
	if (root) {
		return root->info.medie + calculeazaMedieTotala(root->left) + calculeazaMedieTotala(root->right);
	}
	else {
		return 0;
	}
}

float calculeazaMediaStudentilorUneiFacultati(NodArbore* root, const char* facultate) {
	if (root) {
		float suma = 0;
		if (strcmp(root->info.facultate, facultate) == 0) {
			suma = root->info.medie;
		}
		return suma + calculeazaMediaStudentilorUneiFacultati(root->left, facultate) + calculeazaMediaStudentilorUneiFacultati(root->right, facultate);
	}
	else {
		return 0;
	}
}

int main() {
	NodArbore* root = citireArboreDeStudentiDinFisier("studenti.txt");

	afisareStudentiDinArbore(root);

	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(root));
	printf("Numar noduri: %d\n", determinaNumarNoduri(root));
	printf("Suma mediilor: %.2f\n", calculeazaMedieTotala(root));
	printf("Suma medii facultate CSIE: %.2f\n", calculeazaMediaStudentilorUneiFacultati(root, "CSIE"));

	Student cautat = getStudentByID(root, 3);
	if (cautat.id != -1) {
		printf("\nStudent cautat:\n");
		afisareStudent(cautat);
		free(cautat.nume);
		free(cautat.facultate);
	}
	else {
		printf("\nStudentul nu a fost gasit.\n");
	}

	dezalocareArboreDeStudenti(&root);
	return 0;
}
