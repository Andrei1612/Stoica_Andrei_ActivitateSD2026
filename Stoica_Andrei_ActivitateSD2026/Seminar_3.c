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

void afisareMasina(Masina masina) {
	
	printf("ID: %d\n Usi: %d\n Pret: %.2f\n Model: %s\n Nume Sofer: %s\n Serie: %c\n", masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini)
{


	if (masini == NULL || nrMasini == 0)
	{
		printf("Vectorul de masini este gol\n");
		return;
	}
	for (int i = 0; i < nrMasini; i++)
	{
		printf("Masina %d:\n", i + 1);
		afisareMasina(masini[i]);

	}
}


void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua)
{
	Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
	for(int i=0;i<*nrMasini;i++)
	{
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
	

}

Masina citireMasinaFisier(FILE* file) {
	
	char linie[255];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";
	Masina masina;
	masina.id = atoi(strtok(linie, delimitator));
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atof(strtok(NULL, delimitator));
	char* aux = strtok(NULL, delimitator);
	masina.model = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);
	aux= strtok(NULL, delimitator);
	masina.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);
	masina.serie = strtok(NULL, delimitator)[0];
	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {

	FILE* file = fopen(numeFisier, "r");
	if (!file)
	{
		printf("Eroare la deschiderea fisierului\n");
		return;
	}
	else
	{
		Masina* vectorMasini = NULL;
		*nrMasiniCitite = 0;
		while(!feof(file))
		{
			Masina masina = citireMasinaFisier(file);
			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
		}
		return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	if (vector != NULL && *vector != NULL) {
		for (int i = 0; i < *nrMasini; i++) {
			free((*vector)[i].model);
			free((*vector)[i].numeSofer);
		}
		free(*vector);
		*vector = NULL;
		*nrMasini = 0;
	}
}

int main() {
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	printf("--- Datele citite din fisier ---\n");
	afisareVectorMasini(masini, nrMasini);

	dezalocareVectorMasini(&masini, &nrMasini);

	printf("\n--- Dupa dezalocare ---\n");
	afisareVectorMasini(masini, nrMasini);

	return 0;
}