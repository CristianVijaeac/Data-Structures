#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


typedef struct
{
	int ID;	//ID-ul procesului
	int PRIORITY;	//Prioritatea procesului
	int PAS;	//PAS-ul la care procesul este inserat
} TProces;		//Tipul TProces(pentru un proces)

typedef struct cel
{ 
	struct cel *urm;	//Adresa urmatoarei celule din coada 
	TProces* info;	// Informatia unui proces
} TCelulaG, *TLG,**ALG;	//Tipul Celula,Lista Generica 

typedef struct 
{ 
	size_t dime;		//Dimensiunea unui element din stiva
	TLG vf;		//Varful stivei la fel ca toate elementele sunt de tip TLG
} TStiva, *AStiva;	//Tipul Stiva si Pointer la Stiva

typedef struct 
{ 
	size_t dime;	//Dimensiunea unui element din coada
	TLG ic,sc;	//Varful cozii,la fel ca toate elementele sunt de tip TLG
} TCoada, *ACoada;	//Tipul Coada si Pointer la Coada


