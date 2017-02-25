/*VIJAEAC CRISTIAN - 315CB */


#include "CoadaStiva.h"
#define MAXLINE 1000	//Numar maxim caractere linie

void *InitC(size_t de) //Functie de alocare spatiu pentru o coada.
{
	ACoada c=NULL;
	c=(ACoada)malloc(sizeof(TCoada));
	if (!c) return NULL;
	
	c->ic=(TLG)malloc(sizeof(TCelulaG));	//Alocare spatiu pentru inceput.
	if(!c->ic) {
		free(c);
		return NULL;
	}
	
	c->sc=(TLG)malloc(sizeof(TCelulaG));	//Alocare spatiu pentru sfarsit.
	if(!c->sc) {
		free(c->ic);
		free(c);
		return NULL; 
	}
	
	c->ic=NULL;	//Initiale,toate puncteaza spre NULL.
	c->sc=NULL;	
	c->ic=c->sc;
	c->dime=de;
	return (void*)c;
} 

void *InitS(size_t de)	//Functie de alocare spatiu pentru o stiva.
{
	AStiva s=NULL;	
	s=(AStiva)malloc(sizeof(TStiva));
	if (!s) return NULL;		
	
	s->vf=(TLG)malloc(sizeof(TCelulaG));	//Alocare spatiu pentru varful stivei.
	if(!s->vf) {
		free(s);
		return NULL;
	}
	
	s->vf=NULL;	//Toate elementele sunt NULL la inceput.
	s->dime=de;
	return (void*)s;
} 

void DistrugeLG(ALG adrlista)	//Functie de distrugere a unei liste(eliberarea memoriei)
{
	TLG aux=NULL,aL=NULL;
	aL=*adrlista;	//Lucram pe adresa

	while(aL) {	//Cat timp exista lista
		aux=aL;		//Copiem adresa celulei de distrus
		aL=aL->urm;	//Inceputul listei se modifica cu un element(se sare primul)
		free(aux->info);		
		free(aux);	//Eliberam memoria ocupata de toate campurile
	}

	*adrlista=NULL;		//Adresa va puncta catre null
}

void DistrugeC(ACoada *c)	//Functie de distrugere coada.
{	
	DistrugeLG(&(*c)->ic);	//Se distruge lista generica
	
	free((*c)->ic);
	(*c)->ic=NULL;
	
	free((*c)->sc);
	(*c)->sc=NULL;
	
	free(*c);
	*c=NULL;
}

void DistrugeS(AStiva *s,int k)	//Functie de distrugere a tutror stivelor
{
	int i=0;
	TLG p=NULL;

	for(i=0;i<k;i++) {	//Accesam fiecare stiva,pe rand
		p=s[i]->vf;	//Lucram cu adresa listei
		DistrugeLG(&p);		//Distrugem lista
	}

	free((*s)->vf);		
	free(*s);	//Eliberam memoria ocupata de toate campurile
	s=NULL;	   //Adresa va puncta catre null
}

void AfiC(ACoada c,FILE *fw)	//Functie de afisare a cozii.
{
	TLG p=c->ic;
	for(;p;p=p->urm) {			//Parcurgem coada
		fprintf(fw,"%d ",p->info->ID);		//Afisam ID-ul fiecarui element.
	}	
	
	fprintf(fw,"\n");	
	DistrugeLG(&p);	
	return ;	
}

void AfiS(AStiva v,FILE *fw)	//Functie de afisare a stivei.
{
	TLG p=v->vf;
	for(;p;p=p->urm) {	//Parcurgem stiva.
		fprintf(fw,"%d ",p->info->ID);		//Afisam ID-ul fiecarui element.
	}	
	
	fprintf(fw,"\n");	
	DistrugeLG(&p);		
	return ;	
}


TLG ConstrCelula(int ID,int PRIORITY,int i)		//Functia de construire celula COADA
{
	TLG aux=NULL;
	
	aux=(TLG)malloc(sizeof(TCelulaG));	//Alocare spatiu
	if (!aux) {	
		return NULL;	//Alocare esuata
	}
	
	aux->urm=NULL;
	aux->info=(TProces*)malloc(sizeof(TProces));	//Alocare spatiu pentru informatia de tip TProces
	if(!aux->info) {
		free(aux);	//Alocare esuata,elibereaza memoria
		return NULL;
	}

	aux->info->ID=ID;	//Inseram ID in celula cozii.
	aux->info->PRIORITY=PRIORITY;		//Inseram PRIORITY in celula cozii.
	aux->info->PAS=i;
	return aux;
	
}

void Push(TLG ae,AStiva v)	//Functie de inserare element in Stiva.
{
	ae->urm=v->vf;		//Inserarea se face in varful stivei.
	v->vf=ae;
}

TLG Pop(AStiva v)	//Functie de extragere element din Stiva.
{
	TLG elem=NULL,q=v->vf;
	elem=q;		//Extragerea se va face incepand cu elementul din varf.
	v->vf=v->vf->urm;	
	return elem;
}	

void start(int ID,int PRIORITY,int i,ACoada c)		//Functie de construire coada de prioritati.
{	
	TLG aux=NULL,ante=NULL;
	TLG p=c->ic;
	
	aux=ConstrCelula(ID,PRIORITY,i);	//Construire celula pentru fiecare proces.	
	
	if (c->ic==NULL) {		//Cazul in care inseram prima oara in coada.
		c->ic=aux;	
		c->sc=aux;									
		return ;
	}
	else {		
		for(;p;p=p->urm) {	//Daca exista elemente,parcurgem coada.					
			if (p==c->ic && aux->info->PRIORITY>p->info->PRIORITY) {	//Inserare la inceput,atunci cand prioritatea este mai mare
				aux->urm=p;				
				c->ic=aux; 				//Se schimba inceputul.
				return ;
			}
			
			if (p->urm==NULL && aux->info->PRIORITY<=p->info->PRIORITY) {  //Inserare la sfarsit cand prioritatea este mai mica sauegala.
				p->urm=aux;
				c->sc=aux;		//Se schimba sfarsitul.
				return ;
			}			
					
			if (p->info->PRIORITY>=aux->info->PRIORITY &&	//Inserarea in coada,care se va face intotdeauna dupa elementul cu prioritatea >=.
			    p->urm->info->PRIORITY<aux->info->PRIORITY){
				aux->urm=p->urm;				
				p->urm=aux;															
				return ;
			}
				
		}			
		}	
}

void InsOrd(int ID,int PRIORITY,int PAS,ACoada c)	//Functie de reinserare ordonata in functie de PRIORITATE si PASUL initial.
	{
	TLG p=c->ic,ante=NULL;	
	if (c->ic==NULL) {		//Daca coada este vida.
		start(ID,PRIORITY,PAS,c);		//Inseram ca la inceput in coada.
		DistrugeLG(&p);		
		return;	
	}	
	
	if (c->ic!=NULL) {	//Lista nu e vida
		for (;p;ante=p,p=p->urm) {	//O parcurgem
			if (p->info->PRIORITY==PRIORITY) {	//Cazul in care prioritatile coincid si vom tine cont de PAS.
				TLG aux=ConstrCelula(ID,PRIORITY,PAS);
				
				if (p==c->ic && aux->info->PAS<p->info->PAS) {	//Inserare la inceput,avand pasul mai mic.
					aux->urm=p;
					c->ic=aux;				//Se schimba inceputul
					return ;	
				}
			
				if (p!=c->ic && aux->info->PAS<p->info->PAS) {	//Inserarea la mijloc,avand pasul mai mic.
					ante->urm=aux;
					aux->urm=p;				
					return;
				}

				if (p==c->sc && aux->info->PAS>p->info->PAS) { //Inserare la sfarsit,avand pasul mai mare.
					p->urm=aux;
					c->sc=aux;		//Se schimba sfarsitul cozii.
					return;			
				}

				if (p!=c->sc && aux->info->PAS>p->info->PAS && p->urm->info->PRIORITY!=PRIORITY) {	//Inserare la mijloc,avand pasul mai 	mare.			
					aux->urm=p->urm;
					p->urm=aux;
					return;
				}
			}
		}
	}
	if (p==NULL) {		//Daca ajungem la sfarsit si nu s a intrat pe niciunul din cazurile de mai sus
		start(ID,PRIORITY,PAS,c);			//Inseram in functie de prioritate.
		DistrugeLG(&p);			
		return ;
	}					
}

TLG ExtrageC(ACoada a)		//Functie de extragere din coada
{
	TLG p=a->ic,ae=NULL;
	a->ic=a->ic->urm;	//Eliminam elementul.
	ae=p;
	ae->urm=NULL;	
	return ae;
}


void InserareC(ACoada b,TLG ae)		//Functie de inserare in coada
{
	if (b->ic==b->sc && b->ic==NULL) {	//Daca avem coada vida
		b->ic=ae;
		b->sc=ae;			//Inseram ca prim element.
		return ;
	}
	
	ae->urm=b->ic;	//Daca nu,inseram la inceput schimbandu-se inceputul.
	b->ic=ae;
	return;	
}			
			


void wait(AStiva v,ACoada c,int ID)	//Functie de inserare in stiva in functie de ID si eveniment.
{
	TLG ae=NULL;	
	ACoada aux=NULL;
	
	aux=InitC(sizeof(TProces));	//Initializam o coada auxiliara.
	if (!c->ic) {
		DistrugeC(&aux);		//Daca avem coada vida,nu avem ce sa inseram in stiva.
		return ;
	}	
	
	while (c->ic){	//Cat timp avem elemente in coada
		if (c->ic->info->ID==ID) {	//Cautam  ID-ul dorit in coada
			if (c->ic==c->sc && c->ic->urm==NULL) {		//Cazul in care elementul gasit este singurul din coada.
				TLG z=c->ic;	
				c->ic=NULL;	//Coada devine vida
				c->sc=NULL;
				Push(z,v);	//Inseram in stiva elementul.
				break;
			}				
				
			TLG z=c->ic;	//Cazul in care elementul gasit este primul in coada,dar coada are mai multe elemente.	 
			c->ic=c->ic->urm;		//Extragem primul element.
			Push(z,v);		//Inseram in stiva. 
			break;
		}
		
		if (c->ic->info->ID!=ID) {	//Cazul elementelor cu ID uri diferite
			ae=ExtrageC(c);			//Extragem toate elementele cu ID uri diferite
			InserareC(aux,ae);		//Le inseram in coada auxiliara
		}
	}
	if (aux->ic!=NULL) {		//Daca avem coada auxiliara nevida.
		while (aux->ic) {		
			ae=ExtrageC(aux);	//Extragem fiecare element din coada auxiliara.
			InserareC(c,ae);	//Si il reinseram in coada initiala.
		}
	}		
	return ;	
	DistrugeC(&aux);	//Distrugem coada auxiliara.
}



void event(ACoada c,AStiva v)	//Functia de golire a unei stive date si reinserarea elementelor in coada.
{
	TLG q=v->vf;
	TLG elem=NULL;
	
	for(;q;q=q->urm) {	//Parcurgem stiva
		elem=Pop(v);	//Scoatem fiecare element pe rand
		InsOrd(elem->info->ID,elem->info->PRIORITY,elem->info->PAS,c);  //Il reinseram in functie de PRIORITATE si PASUL initial in coada.
	}
	
	DistrugeLG(&q);	
	return;
}



void end(ACoada c,int ID)	//Functia de eliminare a unui element din coada.
	{
	TLG p=c->ic,ante=NULL;
	
	for (;p;ante=p,p=p->urm) {	//Parcurgem coada
		if (p->info->ID==ID) {		//Gasim elementul dorit
			if (p==c->ic && p==c->sc) {	//Cazul in care acesta este singurul element din coada.
				c->ic=NULL;
				c->sc=NULL;		//Coada devine vida.
				DistrugeLG(&p);				
				return ;
			}
			if (p==c->ic && p!=c->sc) {	//Cazul in care elementul este primul.
				c->ic=p->urm;
				p->urm=NULL;		//Se schimba inceputul
				DistrugeLG(&p);				
				return ;			
			}
			if (p!=c->ic && p==c->sc) {
				ante->urm=NULL;		//Cazul in care elementul este ultimul.
				c->sc=ante;		//Se schimba sfarsitul
				DistrugeLG(&p);				
				return;			
			}
			if (p!=c->ic && p!=c->sc) {
				ante->urm=p->urm;
				p->urm=NULL;		//Cazul in care se gaseste undeva in coada.
				DistrugeLG(&p);					
				return ;
			}
		}
	}
}

int main(int argc,char *argv[])		//Citirea parametrilor din linie de comanda
{
	int k=0,ID=0,PRIORITY=0,EVENT=0,i=0,j=0;
	char *line=NULL,*word=NULL;
	FILE *fr=NULL,*fw=NULL;
	ACoada c=NULL;
	AStiva *s=NULL;
	c=InitC(sizeof(TProces));	//Construim coada.
	
	fr=fopen(argv[1],"r");		//Deschiderea fisierului de input
	fw=fopen(argv[2],"w");		//Deschiderea fisierului de output
	
	fscanf(fr,"%d",&k);	//Citim numarul de evenimente
	s=malloc(k*sizeof(AStiva));	//Construim vectorul de stive.
	for (j=0;j<k;j++)
		s[j]=InitS(sizeof(TProces));	//Construim fiecare stiva

	line=(char *)malloc(sizeof(char)*MAXLINE);	//Memoreaza fiecare linie,pe rand
	if (!line) {
		return 0;
	} 

 	while (fgets(line,100, fr)!=NULL) {	//Citim fiecare linie din fisier
		word =strtok (line," \n");	//Extragem primul cuvant de pe linie
			while(word!=NULL) {
    				if (strcmp(word,"start")==0) {		//Daca gasim cuvantul start,apelam functia start		
						ID=atoi(strtok(NULL," \n"));		//Extragem ID
						PRIORITY=atoi(strtok(NULL," \n"));	//Extragem PRIORITY				
						
						i++;			//Incrementam pasul

						start(ID,PRIORITY,i,c);		

						fprintf(fw,"%d\n",i);					
						AfiC(c,fw);			//Afisam coada

						for (j=0;j<k;j++) {
							AStiva q=s[j];							
							if (q->vf!=NULL) {		//Afisam toate stivele nevide.				
								fprintf(fw,"%d: ",j);							
								AfiS(s[j],fw);	
							}		
						}
				}
				if (strcmp(word,"wait")==0) {		//Daca gasim cuvantul wait,apelam functia wait
						EVENT=atoi(strtok(NULL," \n"));	//Extragem evenimentul
						ID=atoi(strtok(NULL," \n"));	//Extragem ID-ul

						wait(s[EVENT],c,ID);

						i++;

						fprintf(fw,"%d\n",i);	
						AfiC(c,fw);		//Afisam coada				

						for (j=0;j<k;j++) {
							AStiva q=s[j];							
							if (q->vf!=NULL) {	//Afisam toate stivele nevide					
								fprintf(fw,"%d: ",j);							
								AfiS(s[j],fw);	
							}		
						}								
				}				
			
				if (strcmp(word,"event")==0) {			//Daca gasim cuvantul event,apelam functia event
						EVENT=atoi(strtok(NULL," \n"));		//Extragem EVENT
						
						event(c,s[EVENT]);	

						i++;

						fprintf(fw,"%d\n",i);	
						AfiC(c,fw);			//Afisam coada

						for (j=0;j<k;j++) {
							AStiva q=s[j];							
							if (q->vf!=NULL) {			//Afisam fiecare stiva nevida				
								fprintf(fw,"%d: ",j);							
								AfiS(s[j],fw);	
							}		
						}
				}

				if (strcmp(word,"end")==0) {	//Daca gasim cuvantul end,apelam functia end
						
						ID=atoi(strtok(NULL," \n"));	

						end(c,ID);				

						i++;

						fprintf(fw,"%d\n",i);	
						AfiC(c,fw);					//Afisam coada

						for (j=0;j<k;j++) {
							AStiva q=s[j];							
							if (q->vf!=NULL) {			//Afisam toate stivele nevide		
								fprintf(fw,"%d: ",j);							
								AfiS(s[j],fw);	
							}		
						}			
				}
				fprintf(fw,"\n");
				word=strtok(NULL," ");	//Trece la urmatoarea linie
				}
		}	
	fclose(fr);
	fclose(fw);	
	
	DistrugeC(&c);	//Eliberam memoria pentru coada
	DistrugeS(s,k);	//Eliberam memoria pentru vectorul de stive

	free(s);
	free(line);		//Eliberam memoria pentru linii.
	return 0;
}
