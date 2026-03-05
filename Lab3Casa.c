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

void afisareCasa(Casa casa) {
	printf("ID:%d\n", casa.id);
	printf("Numar camere:%d\n", casa.nrCamere);
	printf("Pret:%.2f\n", casa.pret);
	printf("Adresa:%s\n", casa.adresa);
	printf("Proprietar:%s\n", casa.proprietar);
	printf("Categorie:%c\n\n", casa.categorie);
}

void afisareVectorCase(Casa* vectorCase, int nrCase) {
	for (int i = 0; i < nrCase; i++) {
		afisareCasa(vectorCase[i]);
	}
}

void adaugaCasaInVector(Casa** vectorCase, int* nrCase, Casa casaNoua) {

	Casa* aux = (Casa*)malloc(sizeof(Casa) * ((*nrCase) + 1));

	for (int i = 0; i < *nrCase; i++) {
		aux[i] = (*vectorCase)[i];
	}

	aux[*nrCase] = casaNoua;

	free(*vectorCase);

	*vectorCase = aux;

	(*nrCase)++;
}

Casa citireCasaFisier(FILE* file) {

	Casa c;

	char buffer[100];
	char sep[4] = ",;\n";

	fgets(buffer, 100, file);

	c.id = atoi(strtok(buffer, sep));
	c.nrCamere = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep);
	c.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.adresa, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.proprietar = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.proprietar, strlen(aux) + 1, aux);

	c.categorie = strtok(NULL, sep)[0];

	return c;
}

Casa* citireVectorCaseFisier(const char* numeFisier, int* nrCaseCitite) {

	FILE* file = fopen(numeFisier, "r");

	Casa* vectorCase = NULL;

	(*nrCaseCitite) = 0;

	while (!feof(file)) {
		adaugaCasaInVector(&vectorCase, nrCaseCitite, citireCasaFisier(file));
	}

	fclose(file);

	return vectorCase;
}

void dezalocareVectorCase(Casa** vectorCase, int* nrCase) {

	for (int i = 0; i < *nrCase; i++) {

		if ((*vectorCase)[i].adresa != NULL) {
			free((*vectorCase)[i].adresa);
		}

		if ((*vectorCase)[i].proprietar != NULL) {
			free((*vectorCase)[i].proprietar);
		}
	}

	free(*vectorCase);

	*vectorCase = NULL;

	*nrCase = 0;
}

int main() {

	int nrCase = 0;

	Casa* vectorCase = citireVectorCaseFisier("case.txt", &nrCase);

	afisareVectorCase(vectorCase, nrCase);

	dezalocareVectorCase(&vectorCase, &nrCase);

	return 0;
}