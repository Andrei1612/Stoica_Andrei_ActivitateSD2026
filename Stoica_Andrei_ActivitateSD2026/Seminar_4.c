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


struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

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
void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	
	Nod * nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	if (*cap == NULL)
	{
		*cap = nodNou;
	}
	else {
		Nod* curent = *cap;
		while (curent->next != NULL)
		{
			curent = curent->next;
		}
		curent->next = nodNou;
	}

}
void afisareListaMasini(Nod* cap) {
	
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}



void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {

	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = *cap;
	*cap = nodNou;
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) {
		printf("Eroare! Nu am gasit fisierul %s\n", numeFisier);
		return NULL;
	}
	Nod* cap = NULL;
	while (!feof(file)) {
		Masina masinaCitita = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&cap, masinaCitita);
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	Nod* curent = *cap;
	while (curent != NULL) {
		Nod* aux = curent->next;
		free(curent->info.model);
		free(curent->info.numeSofer);
		free(curent);
		curent = aux;
	}
	*cap = NULL;
}

float calculeazaPretMediu(Nod* cap) {
	if (cap == NULL) {
		return 0;
	}

	float sumaTotala = 0;
	int numarMasini = 0;
	Nod* curent = cap;
	while (curent != NULL) {
		sumaTotala = sumaTotala + curent->info.pret;
		numarMasini = numarMasini + 1;
		curent = curent->next;
	}
	return sumaTotala / numarMasini;
	
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if ((*cap)) {
		Nod* p = *cap;
		while (p) {
			while (p->next && p->next->info.serie != serieCautata) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.numeSofer) {
					free(aux->info.numeSofer);
				}
				if (aux->info.model) {
					free(aux->info.model);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod*cap, const char* numeSofer) {
	float sumaTotala = 0;
	Nod* curent = cap;
	while (curent != NULL) {
		if (strcmp(curent->info.numeSofer, numeSofer) == 0) {
			sumaTotala = sumaTotala + curent->info.pret;
		}
		curent = curent->next;
	}
	return sumaTotala;

}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("Pretul mediu este:%.2f\n", calculeazaPretMediu(cap));
	printf("Pretul masinilor unui Sofer este:%.2f\n",calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));
	printf("\nStergere seria A\n");
	stergeMasiniDinSeria(&cap, 'A');
	afisareListaMasini(cap);
	printf("\nStergere seria B\n");
	stergeMasiniDinSeria(&cap, 'B');
	afisareListaMasini(cap);
	dezalocareListaMasini(&cap);
	return 0;
}