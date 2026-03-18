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
	struct Nod* prev;      
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;              
	Nod* ultim;           
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasini(ListaDubla lista) {
	Nod* curent = lista.prim;
	while (curent != NULL) {
		afisareMasina(curent->info);
		curent = curent->next;
	}
}
void afisareInversaListaMasini(ListaDubla lista) {
	Nod* curent = lista.ultim;
	while (curent != NULL) {
		afisareMasina(curent->info);
		curent = curent->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL; 
	if (lista->prim == NULL) {
		nodNou->prev = NULL;     
		lista->prim = nodNou;     
		lista->ultim = nodNou;   
	}
	else {
		lista->ultim->next = nodNou;
		nodNou->prev = lista->ultim;
		lista->ultim = nodNou;
	}
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->prev = NULL; 
	if (lista->prim == NULL) {
		nodNou->next = NULL;   
		lista->prim = nodNou;   
		lista->ultim = nodNou; 
	}
	else {
		nodNou->next = lista->prim;
		lista->prim->prev = nodNou;
		lista->prim = nodNou;
	}
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	ListaDubla ld;
	ld.prim = ld.ultim = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
	}
	return ld;
}

void dezalocareLDMasini(ListaDubla* ld) {
	while (ld->prim && ld->prim->next) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);
	}
	ld->prim = ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.prim == NULL) {
		return 0;
	}

	Nod* curent = lista.prim;
	float sumaPreturi = 0;
	int numarMasini = 0;
	while (curent != NULL) {
		sumaPreturi += curent->info.pret; 
		numarMasini++;                     
		curent = curent->next;           
	}
	return sumaPreturi / numarMasini;
}

void stergeMasinaDupaID(ListaDubla* ld, int id) {
	if (ld->prim && ld->prim->info.id == id)
	{
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		if (ld->prim)
		{
			free(ld->prim->prev);
			ld->prim->prev = NULL;
		}
		else {
			free(ld->ultim);
			ld->ultim = NULL;
		}
		return;
	}
	Nod* p = ld->prim;
	while (p && p->info.id != id)
	{
		p = p->next;
	}
	if(p){
		if(p->prev){
			p->prev->next = p->next;
		}
		if(p->next){
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			ld->ultim = p->prev;
		}
		free(p->info.numeSofer);
		free(p->info.model);
		free(p);
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.prim == NULL) {
		return NULL;
	}

	Nod* curent = lista.prim;
	float pretMaxim = curent->info.pret;
	char* soferMasinaScumpa = curent->info.numeSofer;
	while (curent != NULL) {
		if (curent->info.pret > pretMaxim) {
			pretMaxim = curent->info.pret;
			soferMasinaScumpa = curent->info.numeSofer;
		}
		curent = curent->next; 
	}
	return soferMasinaScumpa;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("---------------------------------------\n\n");
	printf("Pretul mediu al masinilor: %.2f\n", calculeazaPretMediu(lista));
	char* soferScump = getNumeSoferMasinaScumpa(lista);
	if (soferScump) {
		printf("Soferul cu cea mai scumpa masina este: %s\n", soferScump);
	}
	printf("---------------------------------------\n\n");
	printf("Dupa stergere masina:\n");
	stergeMasinaDupaID(&lista, 1);
	afisareListaMasini(lista);
	printf("---------------------------------------\n\n");
	printf("Afisare inversa dupa stergere masina:\n\n");
	afisareInversaListaMasini(lista);
	printf("---------------------------------------\n\n");
	dezalocareLDMasini(&lista);

	return 0;
}