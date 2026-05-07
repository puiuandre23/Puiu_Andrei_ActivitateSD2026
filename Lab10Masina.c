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

typedef struct NodArbore NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* right;
	NodArbore* left;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;

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
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
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

void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	if ((*root) != NULL) {
		if (masinaNoua.id > (*root)->info.id) {
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
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
		nou->info = masinaNoua;
		(*root) = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	if (file != NULL) {
		while (!feof(file)) {
			Masina masina = citireMasinaDinFisier(file);
			adaugaMasinaInArboreEchilibrat(&root, masina);
		}
		fclose(file);
	}

	return root;
}

void afisareMasiniDinArborePreordine(NodArbore* root) {
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArborePreordine(root->left);
		afisareMasiniDinArborePreordine(root->right);
	}
}

void afisareMasiniDinArboreInordine(NodArbore* root) {
	if (root) {
		afisareMasiniDinArboreInordine(root->left);
		afisareMasina(root->info);
		afisareMasiniDinArboreInordine(root->right);
	}
}

void afisareMasiniDinArborePostordine(NodArbore* root) {
	if (root) {
		afisareMasiniDinArborePostordine(root->left);
		afisareMasiniDinArborePostordine(root->right);
		afisareMasina(root->info);
	}
}

void afisareMasiniDinArbore(NodArbore* root) {
	printf("\nPREORDINE:\n");
	afisareMasiniDinArborePreordine(root);

	printf("\nINORDINE:\n");
	afisareMasiniDinArboreInordine(root);

	printf("\nPOSTORDINE:\n");
	afisareMasiniDinArborePostordine(root);
}

void dezalocareArboreDeMasini(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);

		free((*root)->info.model);
		free((*root)->info.numeSofer);
		free(*root);
		*root = NULL;
	}
}

Masina getMasinaByID(NodArbore* root, int id) {
	if (root != NULL) {
		if (root->info.id == id) {
			Masina masina;
			masina.id = root->info.id;
			masina.nrUsi = root->info.nrUsi;
			masina.pret = root->info.pret;
			masina.serie = root->info.serie;

			masina.model = (char*)malloc(strlen(root->info.model) + 1);
			strcpy(masina.model, root->info.model);

			masina.numeSofer = (char*)malloc(strlen(root->info.numeSofer) + 1);
			strcpy(masina.numeSofer, root->info.numeSofer);

			return masina;
		}
		else if (id < root->info.id) {
			return getMasinaByID(root->left, id);
		}
		else {
			return getMasinaByID(root->right, id);
		}
	}
	else {
		Masina masina;
		masina.id = -1;
		masina.nrUsi = 0;
		masina.pret = 0;
		masina.model = NULL;
		masina.numeSofer = NULL;
		masina.serie = '-';
		return masina;
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

float calculeazaPretTotal(NodArbore* root) {
	if (root) {
		return root->info.pret + calculeazaPretTotal(root->left) + calculeazaPretTotal(root->right);
	}
	else {
		return 0;
	}
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
	if (root) {
		float suma = 0;
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			suma = root->info.pret;
		}
		return suma + calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) + calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
	}
	else {
		return 0;
	}
}

int main() {
	NodArbore* root = citireArboreDeMasiniDinFisier("masini.txt");

	afisareMasiniDinArbore(root);

	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(root));
	printf("Numar noduri: %d\n", determinaNumarNoduri(root));
	printf("Pret total: %.2f\n", calculeazaPretTotal(root));
	printf("Pret masini sofer Gigel: %.2f\n", calculeazaPretulMasinilorUnuiSofer(root, "Gigel"));

	Masina cautata = getMasinaByID(root, 5);
	if (cautata.id != -1) {
		printf("\nMasina cautata:\n");
		afisareMasina(cautata);
		free(cautata.model);
		free(cautata.numeSofer);
	}
	else {
		printf("\nMasina nu a fost gasita.\n");
	}

	dezalocareArboreDeMasini(&root);
	return 0;
}
