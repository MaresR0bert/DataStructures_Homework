#include <stdio.h>
#include <malloc.h>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

struct Depozit {
	int id = 0;
	char* denumire = NULL;
	char* localitate = NULL;
	float suprafata = 0;
	float capacitate_stocare = 0;
	int nr_ang = 0;
};

struct Nod {
	Depozit* dep = nullptr;
	Nod* next = nullptr;
	Nod* prev = nullptr;
};

struct ListaDubla {
	Nod* first = nullptr;
	Nod* last = nullptr;
};

struct NodArborial {
	Depozit* depo;
	NodArborial* stangu;
	NodArborial* dreptu;
};

ListaDubla* addDep(ListaDubla* listdub, Depozit* dep) {
	Nod* newDepo;
	newDepo = (Nod*)malloc(sizeof(Nod));
	newDepo->dep = (Depozit*)malloc(sizeof(Depozit));
	newDepo->dep->id = dep->id;
	newDepo->dep->denumire = (char*)malloc((sizeof(dep->denumire) + 1) * sizeof(char));
	strcpy(newDepo->dep->denumire, dep->denumire);
	newDepo->dep->localitate = (char*)malloc((sizeof(dep->localitate) + 1) * sizeof(char));
	strcpy(newDepo->dep->localitate, dep->localitate);
	newDepo->dep->capacitate_stocare = dep->capacitate_stocare;
	newDepo->dep->suprafata = dep->suprafata;
	newDepo->dep->nr_ang = dep->nr_ang;
	newDepo->next = NULL;
	newDepo->prev = NULL;

	if (listdub->first == NULL) {
		listdub->first = newDepo;
		listdub->last = newDepo;
	}
	else if (newDepo->dep->id < listdub->first->dep->id) {
		newDepo->next = listdub->first;
		listdub->first->prev = newDepo;
		listdub->first = newDepo;
	}
	else {
		Nod* tempo = listdub->first;
		while (tempo->next != NULL && newDepo->dep->id > tempo->dep->id) tempo = tempo->next;
		if (tempo->next == NULL && newDepo->dep->id > tempo->dep->id) {
			newDepo->prev = tempo;
			listdub->last = newDepo;
			tempo->next = newDepo;
		}
		else {
			newDepo->next = tempo;
			newDepo->prev = tempo->prev;
			tempo->prev->next = newDepo;
			tempo->prev = newDepo;
		}
	}
	return listdub;
}

ListaDubla* modifDen(ListaDubla* listdoub, const char* newDen, int index) {
	Nod* tempo = listdoub->first;
	while (listdoub->first->next != NULL) {
		if (tempo->dep->id == index) {
			strcpy(tempo->dep->denumire, newDen);
			break;
		}
		else {
			tempo = tempo->next;
		}
	}
	return listdoub;
}

float muncitoriMedii(ListaDubla* ld, const char* localitateIdentica) {
	float sum = 0;
	int counter = 0;
	Nod* tempo = ld->first;
	while (tempo != NULL) {
		if (strcmp(tempo->dep->localitate, localitateIdentica) == 0) {
			sum += tempo->dep->nr_ang;
			counter++;
		}
		tempo = tempo->next;
	}
	return sum / counter;
}

NodArborial* creazaNodArobrial(Depozit* depp, NodArborial* stanga, NodArborial* dreapta) {
	NodArborial* tempo = (NodArborial*)malloc(sizeof(NodArborial));
	tempo->depo = depp;
	tempo->stangu = stanga;
	tempo->dreptu = dreapta;
	return tempo;
}

NodArborial* addNodArborial(NodArborial* narbore, Depozit* dep) {
	NodArborial* tempo = NULL;
	if (narbore == NULL) {
		tempo = creazaNodArobrial(dep, NULL, NULL);
	}
	else {
		tempo = narbore;
		while (true) {
			if (dep->id < narbore->depo->id) {
				if (dep->id < narbore->depo->id)
					narbore = narbore->stangu;
				else {
					narbore->stangu = creazaNodArobrial(dep, NULL, NULL);
					return tempo;
				}
			}
			else if (dep->id > narbore->depo->id) {
				if (narbore->dreptu != NULL)
					narbore = narbore->dreptu;
				else {
					narbore->dreptu = creazaNodArobrial(dep, NULL, NULL);
					return tempo;
				}
			}
			else return tempo;

		}
	}
}

NodArborial* listDublaToArboreBinar(ListaDubla* ld) {
	Nod* tempo = ld->first;
	NodArborial* auxi = NULL;
	while (tempo != NULL) {
		auxi = addNodArborial(auxi, tempo->dep);
		tempo = tempo->next;
	}
	return auxi;
}

void afisareListaDubla(ListaDubla* ld) {
	Nod* temp = ld->first;
	while (temp != NULL) {
		printf("DEPOZIT ID: %d, %s, %s, %5.2f, %5.2f, %d\n", temp->dep->id, temp->dep->denumire, temp->dep->localitate, temp->dep->capacitate_stocare, temp->dep->suprafata, temp->dep->nr_ang);
		temp = temp->next;
	}
}

void afisareArboreBinar(NodArborial* nab) {
	if (nab != NULL) {
		afisareArboreBinar(nab->stangu);
		printf("DEPOZIT ID: %d, %s, %s, %5.2f, %5.2f, %d\n", nab->depo->id, nab->depo->denumire, nab->depo->localitate, nab->depo->capacitate_stocare, nab->depo->suprafata, nab->depo->nr_ang);
		afisareArboreBinar(nab->dreptu);
	}
}

NodArborial* modifLoc(NodArborial* nab, int id, const char* localitateNoua) {
	if (nab != NULL) {
		modifLoc(nab->stangu, id, localitateNoua);
		if (nab->depo->id == id) strcpy(nab->depo->localitate, localitateNoua);
		modifLoc(nab->dreptu, id, localitateNoua);
	}
	return nab;
}

void DezalocareLista(ListaDubla* ld) {
	while (ld->first != NULL) {
		free(ld->first->dep->denumire);
		free(ld->first->dep->localitate);
		free(ld->first->dep);
		ld->first = ld->first->next;
	}
	ld->first = NULL;
	ld->last = NULL;
}


int main() {
	ListaDubla L;
	ListaDubla* listaDublaPrincipala = &L;
	listaDublaPrincipala->first = NULL;
	listaDublaPrincipala->last = NULL;
	Depozit* d;
	FILE * f;
	f = fopen("date.txt", "r");

	char* locala, file_buf[128], linesplitter[] = ",";
	while (fgets(file_buf, sizeof(file_buf), f)) {

		d = (Depozit*)malloc(sizeof(Depozit));
		locala = strtok(file_buf, linesplitter);
		d->id = atoi(locala);

		locala = strtok(NULL, linesplitter);
		d->denumire = (char*)malloc((strlen(locala) + 1) * sizeof(char));
		strcpy(d->denumire, locala);

		locala = strtok(NULL, linesplitter);
		d->localitate = (char*)malloc((strlen(locala) + 1) * sizeof(char));
		strcpy(d->localitate, locala);

		locala = strtok(NULL, linesplitter);
		d->capacitate_stocare = atof(locala);

		locala = strtok(NULL, linesplitter);
		d->suprafata = atof(locala);

		locala = strtok(NULL, linesplitter);
		d->nr_ang = atoi(locala);

		listaDublaPrincipala = addDep(listaDublaPrincipala, d);
	}
	fclose(f);

	printf("\n>>>>>>>>>>>>> 1. Lista Dublu inlantuita ordonata crescator dupa ID\n");

	afisareListaDubla(listaDublaPrincipala);

	printf("\n>>>>>>>>>>>>> 2. Modificare denumirii dupa ID\n");

	listaDublaPrincipala = modifDen(listaDublaPrincipala, "Stefan SERELEU", 11);

	afisareListaDubla(listaDublaPrincipala);

	printf("\n>>>>>>>>>>>>> 3. Media angajatiilor dintr-o localitate\n");

	printf("%f\n", muncitoriMedii(listaDublaPrincipala, "Duleni"));

	printf("\n>>>>>>>>>>>>> 4. Arbore binar de cautare\n");

	NodArborial* root;
	root = listDublaToArboreBinar(listaDublaPrincipala);

	afisareArboreBinar(root);

	printf("\n>>>>>>>>>>>>> 5. Modificarea localitatii dupa ID\n");

	root = modifLoc(root, 11, "Grove Street, Home");
	afisareArboreBinar(root);

	printf("\n>>>>>>>>>>>>> 6. Dezalocare memorie\n");

	//DezalocareLista(listaDublaPrincipala); // NU FUNCTIONEAZA => HEAP CORRUPTION

	return 0;
}