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

struct Heap {
	int lungime;
	int nrElemViz;
	Masina* vector;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {

	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (stanga<heap.nrElemViz && heap.vector[stanga].pret > heap.vector[pozMax].pret)
	{
		pozMax = stanga;
	}
	if (dreapta<heap.nrElemViz && heap.vector[dreapta].pret > heap.vector[pozMax].pret)
	{
		pozMax = dreapta;
	}
	if (pozMax != pozitieNod)
	{
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax < ((heap.nrElemViz - 2) / 2))
		{
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	if (f)
	{

		while (!feof(f))
		{
			heap.vector[heap.nrElemViz++] = citireMasinaDinFisier(f);
		}
	}
	fclose(f);
	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	
	return heap;
	
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemViz; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemViz; i < heap.lungime; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	Masina aux;
	aux.id = -1;
	if (heap->nrElemViz > 0)
	{
		heap->nrElemViz--;
		aux=heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = aux;
		filtreazaHeap(*heap, 0);
		
	}
	return aux;
	
	
}


void dezalocareHeap(Heap* heap) {
	
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].numeSofer);
		free(heap->vector[i].model);
		
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrElemViz = 0;
	heap->lungime = 0;

}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("Extrageri!\n");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	printf("Heap ascuns!!\n");
	afiseazaHeapAscuns(heap);
	dezalocareHeap(&heap);
	return 0;
}