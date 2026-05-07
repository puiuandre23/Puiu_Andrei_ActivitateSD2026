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
	unsigned char serie;
};
typedef struct StructuraCasa Casa;

typedef struct NodArbore NodArbore;
struct NodArbore {
	Casa info;
	NodArbore* right;
	NodArbore* left;
};

Casa citireCasaDinFisier(FILE* file) {
	char buffer[150];
	char sep[3] = ",\n";
	fgets(buffer, 150, file);
	char* aux;
	Casa c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);

	aux = strtok(NULL, sep);
	c.nrCamere = atoi(aux);

	aux = strtok(NULL, sep);
	c.pret = (float)atof(aux);

	aux = strtok(NULL, sep);
	c.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy(c.adresa, aux);

	aux = strtok(NULL, sep);
	c.proprietar = (char*)malloc(strlen(aux) + 1);
	strcpy(c.proprietar, aux);

	aux = strtok(NULL, sep);
	c.serie = aux[0];

	return c;
}

void afisareCasa(Casa casa) {
	printf("Id: %d\n", casa.id);
	printf("Nr. camere: %d\n", casa.nrCamere);
	printf("Pret: %.2f\n", casa.pret);
	printf("Adresa: %s\n", casa.adresa);
	printf("Proprietar: %s\n", casa.proprietar);
	printf("Serie: %c\n\n", casa.serie);
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

void adaugaCasaInArboreEchilibrat(NodArbore** root, Casa casaNoua) {
	if ((*root) != NULL) {
		if (casaNoua.id > (*root)->info.id) {
			adaugaCasaInArboreEchilibrat(&(*root)->right, casaNoua);
		}
		else {
			adaugaCasaInArboreEchilibrat(&(*root)->left, casaNoua);
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
		nou->info = casaNoua;
		(*root) = nou;
	}
}

NodArbore* citireArboreDeCaseDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	if (file != NULL) {
		while (!feof(file)) {
			Casa casa = citireCasaDinFisier(file);
			adaugaCasaInArboreEchilibrat(&root, casa);
		}
		fclose(file);
	}

	return root;
}

void afisareCaseDinArborePreordine(NodArbore* root) {
	if (root) {
		afisareCasa(root->info);
		afisareCaseDinArborePreordine(root->left);
		afisareCaseDinArborePreordine(root->right);
	}
}

void afisareCaseDinArboreInordine(NodArbore* root) {
	if (root) {
		afisareCaseDinArboreInordine(root->left);
		afisareCasa(root->info);
		afisareCaseDinArboreInordine(root->right);
	}
}

void afisareCaseDinArborePostordine(NodArbore* root) {
	if (root) {
		afisareCaseDinArborePostordine(root->left);
		afisareCaseDinArborePostordine(root->right);
		afisareCasa(root->info);
	}
}

void afisareCaseDinArbore(NodArbore* root) {
	printf("\nPREORDINE:\n");
	afisareCaseDinArborePreordine(root);

	printf("\nINORDINE:\n");
	afisareCaseDinArboreInordine(root);

	printf("\nPOSTORDINE:\n");
	afisareCaseDinArborePostordine(root);
}

void dezalocareArboreDeCase(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeCase(&(*root)->left);
		dezalocareArboreDeCase(&(*root)->right);

		free((*root)->info.adresa);
		free((*root)->info.proprietar);
		free(*root);
		*root = NULL;
	}
}

Casa getCasaByID(NodArbore* root, int id) {
	if (root != NULL) {
		if (root->info.id == id) {
			Casa casa;
			casa.id = root->info.id;
			casa.nrCamere = root->info.nrCamere;
			casa.pret = root->info.pret;
			casa.serie = root->info.serie;

			casa.adresa = (char*)malloc(strlen(root->info.adresa) + 1);
			strcpy(casa.adresa, root->info.adresa);

			casa.proprietar = (char*)malloc(strlen(root->info.proprietar) + 1);
			strcpy(casa.proprietar, root->info.proprietar);

			return casa;
		}
		else if (id < root->info.id) {
			return getCasaByID(root->left, id);
		}
		else {
			return getCasaByID(root->right, id);
		}
	}
	else {
		Casa casa;
		casa.id = -1;
		casa.nrCamere = 0;
		casa.pret = 0;
		casa.adresa = NULL;
		casa.proprietar = NULL;
		casa.serie = '-';
		return casa;
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

float calculeazaPretulCaselorUnuiProprietar(NodArbore* root, const char* proprietar) {
	if (root) {
		float suma = 0;
		if (strcmp(root->info.proprietar, proprietar) == 0) {
			suma = root->info.pret;
		}
		return suma + calculeazaPretulCaselorUnuiProprietar(root->left, proprietar) + calculeazaPretulCaselorUnuiProprietar(root->right, proprietar);
	}
	else {
		return 0;
	}
}

int main() {
	NodArbore* root = citireArboreDeCaseDinFisier("Case.txt");

	afisareCaseDinArbore(root);

	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(root));
	printf("Numar noduri: %d\n", determinaNumarNoduri(root));
	printf("Pret total: %.2f\n", calculeazaPretTotal(root));
	printf("Pret case proprietar Popescu: %.2f\n", calculeazaPretulCaselorUnuiProprietar(root, "Popescu"));

	Casa cautata = getCasaByID(root, 3);
	if (cautata.id != -1) {
		printf("\nCasa cautata:\n");
		afisareCasa(cautata);
		free(cautata.adresa);
		free(cautata.proprietar);
	}
	else {
		printf("\nCasa nu a fost gasita.\n");
	}

	dezalocareArboreDeCase(&root);
	return 0;
}
