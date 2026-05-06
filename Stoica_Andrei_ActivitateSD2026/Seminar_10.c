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



struct NodArbore {
	Masina info;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};

typedef struct NodArbore Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
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

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL)
		return 0;
	int dr = calculeazaInaltimeArbore(radacina->dreapta);
	int st = calculeazaInaltimeArbore(radacina->stanga);
	return 1 + max(dr, st);
	
}

void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

int verificareEchilibru(Nod* radacina)
{
	return calculeazaInaltimeArbore(radacina->stanga) - calculeazaInaltimeArbore(radacina->dreapta);
}
void adaugaMasinaInArboreEchilibrat(Nod**radacina, Masina masinaNoua) {
	

	if ((*radacina) != NULL)
	{
		if (masinaNoua.id > (*radacina)->info.id)
		{
			adaugaMasinaInArboreEchilibrat(&(*radacina)->dreapta, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*radacina)->stanga, masinaNoua);
		}
		//incepe verificarea echilibrului
		int factorEchilibru = verificareEchilibru(*radacina);
		if (factorEchilibru == -2)
		{
			//dezechilibru in dreapta
			if (verificareEchilibru((*radacina)->dreapta) == -1)
			{
				rotireStanga(&(*radacina));
			}
			else {
				rotireDreapta(&(*radacina)->dreapta);
				rotireStanga(&(*radacina));
			}

		}
		if (factorEchilibru == 2)
		{
			//dezechilibru in stanga
			if (verificareEchilibru((*radacina)->stanga) == -1)
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(&(*radacina));
		}

	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->stanga = NULL;
		nou->dreapta = NULL;
		nou->info = masinaNoua;
		(*radacina) = nou;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* file= fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			adaugaMasinaInArboreEchilibrat(&radacina, citireMasinaDinFisier(file));
		}
	}
	fclose(file);
	return radacina;
}

void afisareMasiniDinArbore(Nod* radacina) {

	if (radacina)
	{
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasiniDinArbore(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina)
	{
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);

		free(*radacina);
		*radacina = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(Nod* radacina, int id) {
	if (radacina == NULL) {
		Masina mGola;
		mGola.id = -1;
		return mGola;
	}

	if (radacina->info.id == id) {
		Masina m;
		m.id = radacina->info.id;
		m.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);

		m.nrUsi = radacina->info.nrUsi;

		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		m.pret = radacina->info.pret;
		m.serie = radacina->info.serie;
		return m;

	}
	if (id < radacina->info.id) {
		return getMasinaByID(radacina->stanga, id);

	}
	if (id > radacina->info.id) {
		return getMasinaByID(radacina->dreapta, id);

	}

}
int determinaNumarNoduri(Nod* radacina) 
{
		if (radacina == NULL)return 0;
		return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

float calculeazaPretTotal(Nod* radacina) 
{
		if (radacina == NULL)return 0;
		return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) 
{
	if (radacina==NULL)return 0;

	float pretCurent=0.0f;
	if (strcmp(numeSofer, radacina->info.numeSofer) == 0) {
		pretCurent = radacina->info.pret;
	}
	return pretCurent + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);


	return 0;
}
int main() {
	Nod* radacina = NULL;

	radacina = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(radacina);

	printf("\nCAUTARE MASINA DUPA ID\n");
	Masina mCautata = getMasinaByID(radacina, 9);
	if (mCautata.id != -1) {
		printf("Masina cu ID-ul %d a fost gasita:\n", 9);
		afisareMasina(mCautata);
		free(mCautata.model);
		free(mCautata.numeSofer);
	}
	else {
		printf("Masina cu ID-ul %d NU a fost gasita.\n", 9);
	}

	printf("\nCALCUL PRET TOTAL\n");
	float pretTotal = calculeazaPretTotal(radacina);
	printf("Valoarea totala a tuturor masinilor: %.2f\n", pretTotal);

	printf("\nPRET MASINI PENTRU UN SOFER\n");
	const char* soferCautat = "Gheorghe"; 
	float pretSofer = calculeazaPretulMasinilorUnuiSofer(radacina, soferCautat);
	printf("Valoarea masinilor conduse de %s: %.2f\n", soferCautat, pretSofer);

	printf("\nDEZALOCARE MEMORIE\n");
	dezalocareArboreDeMasini(&radacina);

	if (radacina == NULL) {
		printf("Arborele a fost dezalocat cu succes\n");
	}
	else {
		afisareMasiniDinArbore(radacina);
	}

	return 0;
}