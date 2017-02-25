#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


typedef struct
{
	char* KEY;	//Adresa numelui
	char* VALUE;	//Adresa IP-ului
} TDNS;		//Tipul DNS(nume,IP)

typedef struct celg
{ 
	struct celg *urm;	//Adresa urmatoarei celule 
	TDNS* info;	// Adresa informatie de tip TDNS
} TCelulaG, *TLG, **ALG;	//Tipurile Celula, Lista Generica si Adresa_Lista_Generica 

typedef int(*TFHASH)(char *,int);	//Functie de dispersie

typedef struct
{ 
	size_t M;	//Nr. maxim de bucket-uri
	TFHASH hashf;	//Functia de dispersie
	TLG *L;		//Adresa Listei Generice
} THash;	//Tipul Tabel "Hash"

