#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include<stdlib.h>
#include<stdio.h>

struct Pacient {
	int id;
	int varsta;
	float gravitate;
	char* nume;
	char* simptom;
};
typedef struct Pacient Pacient;

struct Heap
{
	int lungime;
	int nrElemViz;
	Pacient* vector;
};

typedef struct Heap Heap;
Pacient citirePacient(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, file);
	Pacient p1;
	p1.id = atoi(strtok(buffer, sep));
	p1.varsta = atoi(strtok(NULL, sep));
	p1.gravitate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p1.nume = malloc(strlen(aux) + 1);
	strcpy_s(p1.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p1.simptom = malloc(strlen(aux) + 1);
	strcpy_s(p1.simptom, strlen(aux) + 1, aux);
	return p1;
}

void AfisarePacient(Pacient p)
{
	printf("Id:%d\n", p.id);
	printf("Varsta:%d\n", p.varsta);
	printf("Gravitate:%.2f\n", p.gravitate);
	printf("Nume:%s\n", p.nume);
	printf("Simptom:%s\n\n", p.simptom);
}

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = malloc(sizeof(Pacient) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod)
{
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (stanga<heap.nrElemViz && heap.vector[stanga].gravitate>heap.vector[pozMax].gravitate)
	{
		pozMax = stanga;
	}
	if (dreapta<heap.nrElemViz && heap.vector[dreapta].gravitate>heap.vector[pozMax].gravitate)
	{
		pozMax = dreapta;
	}
	if (pozMax != pozitieNod)
	{
		Pacient aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		filtreazaHeap(heap, pozMax);

	}
}

Heap citirePacientiDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(7);
	if (f)
	{
		while (!feof(f))
		{
			heap.vector[heap.nrElemViz++] = citirePacient(f);
		}
	}
	fclose(f);
	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	return heap;

}

void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nrElemViz; i++)
	{
		AfisarePacient(heap.vector[i]);
	}
}

void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.nrElemViz; i < heap.lungime; i++)
	{
		AfisarePacient(heap.vector[i]);
	}
}

Pacient extragePacient(Heap* heap)
{
	Pacient aux;
	aux.id = -1;
	if (heap->nrElemViz > 0)
	{
		heap->nrElemViz--;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = aux;
		filtreazaHeap(*(heap), 0);
	}
	return aux;
}

void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].nume);
		free(heap->vector[i].simptom);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemViz = 0;
}

int main()
{
	Heap heap = citirePacientiDinFisier("pacienti.txt");
	afisareHeap(heap);
	printf("Extrageri!\n");
	AfisarePacient(extragePacient(&heap));
	AfisarePacient(extragePacient(&heap));
	printf("Heap ascuns\n");
	afisareHeapAscuns(heap);

}