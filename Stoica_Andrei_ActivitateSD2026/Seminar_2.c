#include<stdio.h>
#include<malloc.h>

struct Telefon {
	int id;
	int RAM ;
	char* producator;
	float pret ;
	char serie ;
};
struct Telefon initializare(int id,int RAM,char* producator,float pret,char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}
struct Telefon copiazaTelefon(struct Telefon t)
{
	struct Telefon telefon = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return telefon;
}

void afisare(struct Telefon s) {
	printf("\nId: %d\nRAM: %d\nProducator: %s\nPret: %f\nSerie: %c\n", s.id, s.RAM, s.producator, s.pret, s.serie);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	if (nrElementeCopiate > nrElemente) {
		nrElementeCopiate = nrElemente;
	}
		struct Telefon* vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++)
		{
			vectorNou[i] = copiazaTelefon(vector[i]);
		}

		return vectorNou;
	
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].producator);
		(*vector)[i].producator = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pret_min, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret >= pret_min)
		{
			(*dimensiune)++;
		}

	}
	*vectorNou = malloc(*dimensiune * sizeof(struct Telefon));
	for (int i = 0,j=0; i < nrElemente; i++) {
		if (vector[i].pret >= pret_min) {
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j += 1;
		}
	}


}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon s;
	s.id = 1;

	return s;
}
	


int main() {
	struct Telefon t;
	t = initializare(1, 4, "Samsung", 2000.5, 'A');
	afisare(t);
	int nrTelefoane = 3;
	int nrTelefoaneCopiate = 2;
	struct Telefon* telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0]= initializare(1, 4, "Samsung", 3000.5, 'A');
	telefoane[1]= initializare(2, 4, "Apple", 4000.5, 'S');
	telefoane[2]= t;
	afisareVector(telefoane, nrTelefoane);
	
	struct Telefon* telefoaneCopiate = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);
	printf("\nTelefoane copiate:\n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	dezalocare(&telefoaneCopiate, &nrTelefoaneCopiate);
		printf("\nVector telefoane dupa dezalocare:\n");
	afisareVector(telefoane, nrTelefoane);

	struct Telefon *vectorNou;
	int dimensiune;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2500, &vectorNou, &dimensiune);
	printf("Telefoane scumpe:\n");
	afisareVector(vectorNou, dimensiune);
	return 0;
}