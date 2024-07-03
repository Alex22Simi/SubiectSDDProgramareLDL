#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Programare
{
	char* dataProgramare;
	char* numeClient;
	float durataProgramare;
	char* servicii;
	int cost;
}Programare;
typedef struct NodLs
{
	Programare* info;
	struct NodLs* next;
}NodLs;
typedef struct NodLp
{
	NodLs* info;
	struct NodLp* next;
	char* dataProgramare;
}NodLp;
Programare* initializareProgramare(const char* dataprogramare, const char* numeClient, float durataProgramare, const char* servicii, int cost)
{
	Programare* programare = (Programare*)malloc(sizeof(Programare));
	programare->dataProgramare = (char*)malloc(strlen(dataprogramare) + 1);
	strcpy(programare->dataProgramare, dataprogramare);
	programare->numeClient = (char*)malloc(strlen(numeClient) + 1);
	strcpy(programare->numeClient, numeClient);
	programare->durataProgramare = durataProgramare;
	programare->servicii = (char*)malloc(strlen(servicii) + 1);
	strcpy(programare->servicii, servicii);
	programare->cost = cost;
	return programare;
}
NodLs* initializareNodLs(Programare* programare)
{
	NodLs* nodNou = (NodLs*)malloc(sizeof(NodLs));
	nodNou->info = programare;
	nodNou->next = NULL;
	return nodNou;
}
NodLp* initializareNodLp(const char* dataProgramare, NodLs* nodLs)
{
	NodLp* nodNou = (NodLp*)malloc(sizeof(NodLp));
	nodNou->dataProgramare = (char*)malloc(strlen(dataProgramare) + 1);
	strcpy(nodNou->dataProgramare, dataProgramare);
	nodNou->info = initializareNodLs(nodLs->info);
	nodNou->next = NULL;
	return nodNou;
}
NodLs* inserareInListaSecundara(NodLs* cap, Programare* programare)
{
	NodLs* nodNou = initializareNodLs(programare);
	if (cap == NULL)
	{
		return nodNou;
	}
	else
	{
		NodLs* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nodNou;
	}
	return cap;
}
NodLp* inserareInListPrincipala(NodLp* cap, const char* dataProgramare,NodLs* nodLs)
{
	NodLp* nodNou = initializareNodLp(dataProgramare, nodLs);
	if (cap == NULL)
	{
		return nodNou;

	}
	else
	{
		NodLp* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nodNou;
	}
	return cap;
}
void afisareLs(NodLs* cap)
{
	if (cap != NULL)
	{
		while (cap != NULL)

		{
			printf("%s %s %f %s %d\n", cap->info->dataProgramare, cap->info->numeClient,
				cap->info->durataProgramare, cap->info->servicii, cap->info->cost);
			cap = cap->next;
		}
		
	}
}
void afisareLp(NodLp* cap)
{
	if (cap != NULL)
	{
		while (cap != NULL)
		{
			printf("Data programarii: %s\n", cap->dataProgramare);
			afisareLs(cap->info);
			cap = cap->next;
		}
	}
}
void dezalocareLs(NodLs* cap)
{
	NodLs* temp = cap;
	while (temp != NULL)
	{
		NodLs* nodDeSters = temp;
		temp = temp->next;
		free(temp->info->dataProgramare);
		free(temp->info->numeClient);
		free(temp->info->servicii);
		free(temp->info);
		free(temp);
		temp = nodDeSters;
	}
}
void dezalocareLp(NodLp* cap)
{
	NodLp* temp = cap;
	while (temp != NULL)
	{
		NodLp* nodDeSters = temp;
		temp = temp->next;
		dezalocareLs(temp->info);
		free(temp->dataProgramare);
		free(temp);
		temp = nodDeSters;
	}
}
//det val tot a programarilor efectuate la o anumita data, data e specificata ca param
int valTotalaProgramari(NodLp* cap, const char* dataProgramare)
{
	NodLp* temp = cap;
	while (temp != NULL && strcmp(temp->dataProgramare, dataProgramare) != 0)
	{
		temp = temp->next;
	}
	if (temp == NULL)
	{
		return 0;
	}
	int total = 0;
	NodLs* tempLs = temp->info;
	while (tempLs != NULL)
	{
		total += tempLs->info->cost;
		tempLs = tempLs->next;
	}
	return total;
}
//modifica costul unei programari specificata prin numele clientului specificat in param, costul nou este 
//specificat in param
void modificaPret(NodLp* cap, const char* numeClient, int costNou)
{
	NodLp* temp = cap;
	while (temp != NULL)
	{
		NodLs* tempLs = temp->info;//parcurgere liste secundare
		while (tempLs != NULL)
		{
			if (strcmp(tempLs->info->numeClient, numeClient) == 0)
			{
				tempLs->info->cost = costNou;
			}
			tempLs = tempLs->next;
		}
		temp = temp->next;//parcurgere lista principala
	}
}
//stergere programari ale caror durata este mai mica decat un prag dat ca parametru
void stergeProgramariMaiScurte(NodLp* cap, float prag)
{
	NodLp* tempLp = cap;
	NodLp* anteriorLp = NULL;//stergem temp-ul anterior

	while (tempLp != NULL)
	{
		NodLs* tempLs = tempLp->info;
		NodLs* anteriorLs = NULL;

		while (tempLs != NULL)
		{
			if (tempLs->info->durataProgramare < prag)
			{
				if (anteriorLs == NULL) // dacă nodul de sters este primul in lista secundara
				{
					tempLp->info = tempLs->next;//face legatura intre nodul curent si nodul urmator
				}
				else
				{
					anteriorLs->next = tempLs->next;//cazul in care nu ne aflam pe primul nod
				}

				NodLs* nodDeSters = tempLs;
				tempLs = tempLs->next;

				// Eliberare memorie
				free(nodDeSters->info->dataProgramare);
				free(nodDeSters->info->numeClient);
				free(nodDeSters->info->servicii);
				free(nodDeSters->info);
				free(nodDeSters);
			}
			else
			{
				anteriorLs = tempLs;
				tempLs = tempLs->next;
			}
		}

		// Verificăm dacă lista secundară este goală după ștergeri
		if (tempLp->info == NULL)
		{
			if (anteriorLp == NULL) // dacă nodul de șters este primul în lista principală
			{
				cap = tempLp->next;
			}
			else
			{
				anteriorLp->next = tempLp->next;
			}

			NodLp* nodDeSters = tempLp;
			tempLp = tempLp->next;

			// Eliberare memorie
			free(nodDeSters->dataProgramare);
			free(nodDeSters);
		}
		else
		{
			anteriorLp = tempLp;
			tempLp = tempLp->next;
		}
	}
}


void main()
{
	FILE* f = fopen("Programare.txt", "r");
	if (f != NULL)
	{
		char bufferLine[300], sep_list[] = ",\n";
		NodLp* capLp = NULL;

		while (fgets(bufferLine, sizeof(bufferLine), f))
		{
			char* token = strtok(bufferLine, sep_list);
			char* dataProgramare = (char*)malloc(strlen(token) + 1);
			strcpy(dataProgramare, token);

			token = strtok(NULL, sep_list);
			char* numeClient = (char*)malloc(strlen(token) + 1);
			strcpy(numeClient, token);

			token = strtok(NULL, sep_list);
			float durataProgramare = (float)atof(token);

			token = strtok(NULL, sep_list);
			char* servicii = (char*)malloc(strlen(token) + 1);
			strcpy(servicii, token);

			token = strtok(NULL, sep_list);
			int cost = (int)atoi(token);

			Programare* programare = initializareProgramare(dataProgramare, numeClient, durataProgramare, servicii,cost);
			NodLp* temp = capLp;
			//NodLp* anterior = NULL;

			while (temp != NULL && strcmp(temp->dataProgramare, dataProgramare) != 0)
			{
				//anterior = temp;
				temp = temp->next;
			}
			if (temp == NULL)
			{
				NodLs* capLs = NULL;
				capLs = inserareInListaSecundara(capLs, programare);
				capLp = inserareInListPrincipala(capLp, dataProgramare, capLs);
			}
			else
			{
				temp->info = inserareInListaSecundara(temp->info, programare);
			}
		
		}
		fclose(f);
		afisareLp(capLp);

		printf("Cerinta 2: \n");
		int valTotala = 0;
		valTotala = valTotalaProgramari(capLp, "12.06.2024");
		printf("%d\n",valTotala);

		printf("Cerinta 3: \n");
		modificaPret(capLp,"Alex", 750);
		afisareLp(capLp);

		printf("Cerinta 4: \n");
		stergeProgramariMaiScurte(capLp, 5);
		afisareLp(capLp);
	}
}