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

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina m;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

void inserareListaP(NodP** graf, Masina m) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->m = m;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*graf) {
		NodP* aux = *graf;
		while (aux->next) {
			aux = aux->next;

		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareListaS(NodS** lista, NodP* vecin) {

	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;

	if (*lista) {
		NodS* aux = *lista;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;

	}
	else {
		*lista = nou;
	}
}

NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* temp = listaPrincipala;
	while (temp) {
		if (temp->m.id == id) return temp;
		temp = temp->next;
	}
	return NULL;


}

void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {

	NodP* start = cautaNodDupaID(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaID(listaPrincipala, idStop);
	if (start && stop) {
		inserareListaS(&start->vecini, stop);
		inserareListaS(&stop->vecini, start);
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;
	while (!feof(f)) {
		inserareListaP(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		int idStart;
		int idStop;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(graf, idStart, idStop);
	}
	fclose(f);
}

void afisareListaVecini(NodP* graf, int id) {
	NodP* temp = cautaNodDupaID(graf, id);
	NodS* cap = temp->vecini;
	while (cap) {
		afisareMasina(cap->info->m);
		cap = cap->next;
	}
}

void dezalocareNoduriGraf(NodP** listaPrincipala) {
	if (listaPrincipala == NULL || *listaPrincipala == NULL) return;

	NodP* auxP = *listaPrincipala;
	while (auxP != NULL) {
		NodS* auxS = auxP->vecini;
		while (auxS != NULL) {
			NodS* stergereS = auxS;
			auxS = auxS->next;
			free(stergereS);
		}

		if (auxP->m.model) {
			free(auxP->m.model);
		}
		if (auxP->m.numeSofer) {
			free(auxP->m.numeSofer);
		}

		NodP* stergereP = auxP;
		auxP = auxP->next;
		free(stergereP);
	}

	*listaPrincipala = NULL;
}

int main() {
	NodP* graf;
	graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);
	afisareListaVecini(graf, 3);


	dezalocareNoduriGraf(&graf);

	return 0;
}