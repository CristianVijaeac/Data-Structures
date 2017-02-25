#include "HashTable.h"
#define MAXLINE 1000	//Numar maxim caractere linie
#define MAXLETTER 100	//Numar maxim caractere nume/IP


int hashf(char *KEY,int M)	//Functia de dispersie
{
	int i=0,l=0,res=0,sum=0;

	l=strlen(KEY);	//Numarul de caractere

	for (i=0;i<l;i++) {
		sum=sum+KEY[i];	//Calculam suma caracterelor
	}	
	res=sum%M;	//Returnam restul dintre suma caractere si M

	return res;
}





THash *InitHash(int M,TFHASH hashf)	//Functia de alocare spatiu pentru HashTabel
{
	THash *H=NULL;
	int i=0;

	H=(THash*)malloc(sizeof(THash));	//Alocare spatiu total
	if(!H) {
		return NULL;	//Alocare esuata
	}

	H->L=(TLG*)calloc(M,sizeof(TLG));	//Alocare spatiu pentru fiecare bucket
	for(i=0;i<M;i++) {
		H->L[i]=NULL;			//Initial,bucket-ul este NULL
	}	
	if(!H->L) {
		free(H);	//Alocare esuata,elibereaza memoria ocupata anterior
		return NULL;
	}

	H->M=M;		//Numarul maxim de bucket-uri
	H->hashf=hashf;	//Functia de dispersie

	return H;
}





TLG ConstrCelula(char *KEY,char *VALUE)		//Functia de construire celula de tip TCelulaG
{
	TLG aux=NULL;
	
	aux=(TLG)malloc(sizeof(TCelulaG));	//Alocare spatiu
	if (!aux) {	
		return NULL;	//Alocare esuata
	}
	
	aux->urm=NULL;
	aux->info=(TDNS*)malloc(sizeof(TDNS));	//Alocare spatiu pentru informatia de tip TDNS
	if(!aux->info) {
		free(aux);	//Alocare esuata,elibereaza memoria
		return NULL;
	}
	
	aux->info->KEY=malloc(MAXLETTER);	//Alocare spatiu pentru nume
	if(!aux->info->KEY) {
		free(aux->info);	//Alocare esuata,elibereaza toata memoria anterioara
		free(aux);
		return NULL;
	}

	aux->info->VALUE=malloc(MAXLETTER);	//Alocare spatiu pentru IP
	if(!aux->info->VALUE) {
		free(aux->info);	//Alocare esuata,elibereaza toata memoria anterioara
		free(aux);
		return NULL;
	}

	strcpy(aux->info->KEY,KEY);	//Copiem sirul KEY in spatiul alocat pentru acesta
	strcpy(aux->info->VALUE,VALUE);		//Copiem sirul VALUE in spatiul alocat pentru acesta

	return aux;
	
}





void put(char *KEY,char *VALUE,THash *H,int M)		//Functie de inserare a unui element in HashTabel
{
	int k=0;
	TLG aux=NULL,ante=NULL;		//ante memoreaza celula anterioara

	aux=ConstrCelula(KEY,VALUE);	//Construim celula ce trebuie inserata

	k=hashf(KEY,M);		//Aflam bucket-ul in care trebuie plasata celula
	TLG p=H->L[k];		//Pointer la lista aferenta bucket-ului

	if (!p) {			//Daca lista este goala
		H->L[k]=aux;		//Inseram la inceput,ca prim element
	} else {
		for(;p;ante=p,p=p->urm) {	//Daca nu,parcurgem lista
			if (strcmp(p->info->KEY,aux->info->KEY)==0) {	//Verificam daca elementul exista deja in lista
				free(aux->info->KEY);	
				free(aux->info->VALUE);
				free(aux->info);
				free(aux);	//Daca da,eliberam toata memoria ocupata de celula aux
				return ;
			} else {			
				if(strcmp(p->info->KEY,aux->info->KEY)>0) { //Daca compararea returneaza >0,aux trebuie plasata inainte de p
					if (p==H->L[k]) {	//In cazul in care p reprezinta prima celula din lista,inseram la inceput
						H->L[k]=aux;	//Legam noua celula de inceputul listei	
						aux->urm=p;	//Legam noua celula de celula deja existenta
						return ;
					} else {
						ante->urm=aux;	//Daca p nu este prima celula,aux se plaseaza intre ante
						aux->urm=p;//si p
						return ;
					}				
				}
			}		
		}
		if (!p) {		//Ajungem la sfarsit,celula trebuie inserata la sfarsit		
			ante->urm=aux;
			return ;
		}
	}
}





void print(THash *H,int M,FILE *fw)	//Functie de afisare a tuturor IP urilor din bucket-uri
{
	int i=0;

	for (i=0;i<M;i++) {	
		TLG p=H->L[i];	//Luam fiecare bucket,pe rand
		if (p) {	
			fprintf(fw,"%d: ",i);	//Afisam indicele bucketului
			for(;p;p=p->urm) {
				fprintf(fw,"%s ",p->info->VALUE);	//Afisam IP-ul aflat in fiecare celula din lista
			}
			fprintf(fw,"\n");	
		}
	}
}





void print_bucket(THash *H,int k,FILE *fw)	//Functie care returneaza IP-urile din lista aflata in bucketul "k"
{
	TLG p=H->L[k];	//Pointer la lista ceruta

	if (!p) {	
		fprintf(fw,"VIDA\n");	//Daca lista nu exista(bucketul e NULL) afiseaza VIDA
		return ;
	} else {
		for(;p;p=p->urm) {
			fprintf(fw,"%s ",p->info->VALUE);	//Daca nu,afiseaza IP-ul din fiecare celula a listei
		}	
		fprintf(fw,"\n");
		return ;	
	}
}





char *get(THash *H,char* KEY,int M)	//Functie care returneaza valoarea aferenta(IP-ul) a unui nume (KEY) dat
{
	int k=0;
	k=hashf(KEY,M);		//Indicele bucket-ului in care se poate afla numele(KEY)
	TLG p=H->L[k];		//Pointer la lista gasita

	for(;p;p=p->urm) {
		if (strcmp(p->info->KEY,KEY)==0) {	//Daca numele este gasit in lista
			return p->info->VALUE;		//returneaza IP-ul
		}
	}
	
	return NULL;	//Daca nu,returneaza null
}





int find(THash *H,char* KEY,int M)	//Functie care cauta un nume dat(KEY) in HashTable
{
	int k=0;	
	k=hashf(KEY,M);		//Indicele bucket-ului in care este posibil sa se afle numele
	TLG p=H->L[k];		//Pointer la lista gasita

	for(;p;p=p->urm) {
		if (strcmp(p->info->KEY,KEY)==0) {	//Daca numele este gasit in lista returneaza 1,daca nu returneaza 0
			return 1;			
		}
	}	
	
	return 0;
} 





void Remove(THash *H,int M,char* KEY)		//Functie care inlatura o celula din lista in functie de un nume(KEY) dat
{
	int k=0;
	k=hashf(KEY,M);		//Indicele bucket-ului in care se afla celula cautata
	TLG p=H->L[k],ant=NULL;		//Pointer la lista gasita,ant=pointer la celula anterioara
	
	for (;p;ant=p,p=p->urm) {		
		if (strcmp(p->info->KEY,KEY)==0) {	//Daca elementul este gasit
			if(p==H->L[k]) {		//Daca pozitia element=inceput de lista
				H->L[k]=p->urm; //Bucket-ul devine null(p->urm este null prin constructie)
				free(p->info->KEY);
				free(p->info->VALUE);
				free(p->info);
				free(p);	//Eliberam memoria ocupata de Celula
				return;
			} else {
				if(p->urm==NULL) {	//Daca elementul gasit se afla la sfarsit
					ant->urm=NULL; //Elementul anterior lui va puncta direct la null
					free(p->info->KEY);
					free(p->info->VALUE);
					free(p->info);
					free(p);	//Eliberam memoria ocupata de Celula
					return;
				} else {	//In cazul in care celula se afla la mijloc
					ant->urm=p->urm; 	//Se sare peste celula gasita(dupa cel anterioara urmeaza cel urmatoare)
					free(p->info->KEY);
					free(p->info->VALUE);
					free(p->info);
					free(p);	//Eliberam memoria ocupata de Celula
					return;
				}			
						
				}
		}
	}
}





void DistrugeLG(ALG adrlista)	//Functie de distrugere a unei liste(eliberarea memoriei)
{
	TLG aux=NULL,aL=NULL;
	aL=*adrlista;	//Lucram pe adresa

	while(aL) {	//Cat timp exista lista
		aux=aL;		//Copiem adresa celulei de distrus
		aL=aL->urm;	//Inceputul listei se modifica cu un element(se sare primul)
		free(aux->info->VALUE);		
		free(aux->info->KEY);
		free(aux->info);		
		free(aux);	//Eliberam memoria ocupata de toate campurile
	}

	*adrlista=NULL;		//Adresa va puncta catre null
}





void DistrugeTHash(THash **H,int M)	//Functie de distrugere a HashTabel
{
	int k=0;
	TLG p=NULL;

	for(k=0;k<M;k++) {	//Accesam fiecare bucket,pe rand
		p=(*H)->L[k];	//Lucram cu adresa listei
		DistrugeLG(&p);		//Distrugem lista
	}

	free((*H)->L);		
	free(*H);	//Eliberam memoria ocupata de toate campurile
	*H=NULL;	//Adresa va puncta catre null
}





int main(int argc,char *argv[])		//Citirea parametrilor din linie de comanda
{
	int M=0,k,res=0;
	char *line=NULL,*word=NULL,*KEY=NULL,*VALUE=NULL,*afis=NULL;
	FILE *fr=NULL,*fw=NULL;
	THash *H=NULL;
	
	M=atoi(argv[1]);	//Extragerea nr. de bucket-uri
	H=InitHash(M,hashf);	//Alocarea spatiului si formarea HashTabel
	
	fr=fopen(argv[2],"r");		//Deschiderea fisierului de input
	fw=fopen(argv[3],"w");		//Deschiderea fisierului de output
	
	line=(char *)malloc(sizeof(char)*MAXLINE);	//Memoreaza fiecare linie,pe rand
	if (!line) {
		return 0;
	} 

 	while (fgets(line,100, fr)!=NULL) {	//Citim fiecare linie din fisier
		word =strtok (line," \n");	//Extragem primul cuvant de pe linie
			while(word!=NULL) {
    				if (strcmp(word,"put")==0) {	//Daca cuvantul este put,executam comanda			
						KEY=strtok(NULL," \n");		//Extragem al doilea sir din linie
						VALUE=strtok(NULL," \n");	//Extragem al treilea sir din linie
						put(KEY,VALUE,H,M);		//Functia
				}
				
				if (strcmp(word,"get")==0) {	//Daca cuvantul este get,executam comanda
						KEY=strtok(NULL," \n");		//Extragem al doilea sir din linie
						afis=get(H,KEY,M);		//Functia
							if (afis) {
 								fprintf(fw,"%s\n",afis);	//Scriem in fisier ceea ce returneaza functia
							
							} else {
								fprintf(fw,"NULL\n");		
							}					
				}
		
				if (strcmp(word,"remove")==0) {		//Daca cuvantul este remove,executam comanda
						KEY=strtok(NULL," \n");		//Extragem al doilea cuvant din sir
						Remove(H,M,KEY);	//Functia
				}

				if (strcmp(word,"find")==0) {		//Daca cuvantul este find,executam comanda
						KEY=strtok(NULL," \n");		//Extragem al doilea cuvant din sir	
						res=find(H,KEY,M);	//Functia
						if (res==1) {
							fprintf(fw,"True\n");		//Daca numele(KEY) este gasit,returneaza "TRUE"
						} else {
							fprintf(fw,"False\n");			//Daca nu,"FALSE"
						}
				}	

				if (strcmp(word,"print")==0) {		//Daca cuvantul este print,executam comanda
					print(H,M,fw);		//Functia
				}
				
				if (strcmp(word,"print_bucket")==0) {	//Daca cuvantul este print_bucket,executam comanda
						k=atoi(strtok(NULL," \n"));	//Extragem intregul k(indicele bucketului)
						if(k>=0 && k<M) {	//k mai mic decat numarul maxim de bucket-uri
							print_bucket(H,k,fw);			//Functia
						}
			 	}
				word=strtok(NULL," ");	//Trecem la urmatoarea linie
				}
		}	
	fclose(fr);
	fclose(fw);
	DistrugeTHash(&H,M);	//Distrugem HashTabel
	free(line);		//ELiberam spatiul ocupat de linii
	return 0;
}
