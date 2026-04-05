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
	m1.pret = (float)atof(strtok(NULL, sep));

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


typedef struct Nod Nod;
struct Nod {
	Masina masina;
	Nod* next;
};

void pushStack(Nod** cap, Masina masina) {
	Nod* first = (Nod*)malloc(sizeof(Nod));
	first->masina = masina;
	first->next = *cap;
	*cap = first;
}

Masina popStack(Nod** cap) {
	if ((*cap) == NULL) {
		Masina rezultat;
		rezultat.id = -1;
		return rezultat;
	}
	Masina rezultat = (*cap)->masina;
	Nod* temp = (*cap)->next;
	free(*cap);
	*cap = temp;
	return rezultat;
}

int isEmptyStack(Nod* cap) {
	return cap == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		return NULL;
	}
	Nod* cap = NULL;
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		pushStack(&cap, masina);
	}
	fclose(file);
	return cap;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while (!isEmptyStack(*stiva)) {
		Masina masinaCurenta = popStack(stiva);
		free(masinaCurenta.model);
		free(masinaCurenta.numeSofer);
	}
	printf("Stiva de masini a fost dezalocata cu succes!\n");
}

int size(Nod** stiva) {
	int numarElemente = 0;
	Nod* stivaTemporara = NULL;
	while (!isEmptyStack(*stiva)) {
		Masina masinaCurenta = popStack(stiva);
		pushStack(&stivaTemporara, masinaCurenta);
		numarElemente++;
	}
	while (!isEmptyStack(stivaTemporara)) {
		pushStack(stiva, popStack(&stivaTemporara));
	}
	return numarElemente;
}

Masina getMasinaByID(Nod** stiva, int id) {
	Masina rezultat;
	rezultat.id = -1;

	if ((*stiva) == NULL) return rezultat;

	Nod* stivaNoua = NULL;

	while ((*stiva)) {
		Masina masinaCurenta = popStack(stiva);
		pushStack(&stivaNoua, masinaCurenta);

		if (masinaCurenta.id == id) {
			rezultat = masinaCurenta;
			break;
		}
	}
	while (stivaNoua) {
		pushStack(stiva, popStack(&stivaNoua));
	}

	return rezultat;
}

float calculeazaPretTotal(Nod** stiva) {
	float pretTotal = 0.0f;
	Nod* stivaTemporara = NULL;
	while (!isEmptyStack(*stiva)) {
		Masina masinaCurenta = popStack(stiva);
		pretTotal += masinaCurenta.pret;
		pushStack(&stivaTemporara, masinaCurenta);
	}
	while (!isEmptyStack(stivaTemporara)) {
		pushStack(stiva, popStack(&stivaTemporara));
	}
	return pretTotal;
}

typedef struct NodDublu NodDublu;
struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

typedef struct ListaDubluInlantuita ListaD;
struct ListaDubluInlantuita {
	NodDublu* first;
	NodDublu* last;
};

void enqueue(ListaD* coada, Masina masina) {
	NodDublu* newNod = malloc(sizeof(NodDublu));
	newNod->info = masina;
	newNod->next = NULL;
	newNod->prev = coada->last;

	if (coada->last) {
		coada->last->next = newNod;
	}
	else {
		coada->first = newNod;
	}
	coada->last = newNod;
}

Masina dequeue(ListaD* coada) {
	Masina rezultat;
	rezultat.id = -1;

	if (coada->first) {
		rezultat = coada->first->info;
		NodDublu* temp = coada->first;
		coada->first = temp->next;

		if (coada->first != NULL) {
			coada->first->prev = NULL;
		}
		else {
			coada->last = NULL;
		}
		free(temp);
	}
	return rezultat;
}

ListaD citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	ListaD coada;
	coada.first = coada.last = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			enqueue(&coada, citireMasinaDinFisier(f));
		}
		fclose(f);
	}
	return coada;
}

void dezalocareCoadaDeMasini(ListaD* coada) {
	while (coada->first != NULL) {
		Masina masinaCurenta = dequeue(coada);
		if (masinaCurenta.id != -1) {
			free(masinaCurenta.model);
			free(masinaCurenta.numeSofer);
		}
	}
	printf("Coada de masini a fost dezalocata cu succes!\n");
}

int main() {
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");

	printf("Avem %d masini in stiva.\n", size(&stiva));
	printf("Pretul total: %.2f\n", calculeazaPretTotal(&stiva));

	printf("\nCautare masina cu ID 7:\n");
	afisareMasina(getMasinaByID(&stiva, 7));

	printf("Scoatem primele 2 masini din stiva (pop):\n");
	Masina m1 = popStack(&stiva);
	afisareMasina(m1);
	if (m1.id != -1) { free(m1.model); free(m1.numeSofer); }

	Masina m2 = popStack(&stiva);
	afisareMasina(m2);
	if (m2.id != -1) { free(m2.model); free(m2.numeSofer); }

	dezalocareStivaDeMasini(&stiva);

	ListaD coada = citireCoadaDeMasiniDinFisier("masini.txt");

	printf("Scoatem prima masina din coada :\n");
	Masina m3 = dequeue(&coada);
	afisareMasina(m3);
	if (m3.id != -1) { free(m3.model); free(m3.numeSofer); }

	dezalocareCoadaDeMasini(&coada);

	return 0;
}