/* VIJAEAC CRISTIAN-OCTAVIAN 315CB */


#include "StructuraArbore.h"
#define MAXLINE 100

TArb ConstrFrunza(int a,int b,int c,int type)				
	{ 
		TArb aux = (TArb)malloc (sizeof(TNod)); //alocare memorie 
  	
		if (!aux) return NULL;                   //alocare nereusita

	  	aux->a=a;		//inserarea elementelor in frunza
		aux->b=b;
		aux->c=c;
		aux->type=type;

		aux->st =aux->dr =NULL; 	//fiind frunza,stanga respectiv dreapta devin null
		 
	 	return aux;                              
	}

TArb ConstrArboreDr_1(TArb arb,TArb *aa,FILE *fr,int dreapta[100][3])
	{
		int ind_nod=0,ind_st=0,ind_dr=0;
	
		fscanf(fr,"%d",&ind_nod);	//citeste din fisier indicele dreptei ce urmeaza a fi inserata
		fscanf(fr,"%d",&ind_st);	//indice de existenta al fiului stang
		fscanf(fr,"%d",&ind_dr);	//indice de existenta al fiului drept
		
			if (!(*aa)) {
				(*aa)=ConstrFrunza(dreapta[ind_nod][0],dreapta[ind_nod][1],dreapta[ind_nod][2],0);	
				//daca ajungem la un element NULL,construim frunza aici				
			}
		
			if (ind_st == 1 && ind_dr == 0) {
				ConstrArboreDr_1(arb,&(*aa)->st,fr,dreapta);
				//daca exista fiu stang,parcurgem in continuare arborele pe partea stanga			
			}
			
			if (ind_st == 0 && ind_dr == 1) {
				ConstrArboreDr_1(arb,&(*aa)->dr,fr,dreapta);
				//daca exista fiu drept,parcurgem in continuare arborele pe partea dreapta			
			}
	
			if (ind_st == 1 && ind_dr == 1) {
				ConstrArboreDr_1(arb,&(*aa)->st,fr,dreapta);
				ConstrArboreDr_1(arb,&(*aa)->dr,fr,dreapta);
				//daca exista ambii fii,parcurgem arborele mai intai pe stanga,apoi pe dreapta			
			}
			
			if (ind_st == 0 && ind_dr == 0) {
				(*aa)->st=NULL;
				(*aa)->dr=NULL;		
				//daca niciunul din fii nu exista,inseamna ca acestia devin NULL
			}
	
		return arb;
	}

TArb ConstrArboreDr_2(TArb arb,TArb *aa,int a,int b,int c)
	{
		if (!(*aa)) {
			(*aa)=ConstrFrunza(a,b,c,0);	//daca ajungem la un element NULL,aici vom construii noua frunza
		}
	
		if (((float)-(*aa)->a ) / (*aa)->b == (float)(-a) / b) {	//cazul in care pantele sunt egale(drepte paralele)
			if (c > (*aa)->c) {
				ConstrArboreDr_2(arb,&(*aa)->st,a,b,c);	//in cazul in care dreapta ce urmeaza a fi inserata are "c" mai mare,mergem pe stanga
			}
			if (c < (*aa)->c) {
				ConstrArboreDr_2(arb,&(*aa)->dr,a,b,c);	//in cazul in care dreapta ce urmeaza a fi inserata are "c" mai mic,mergem pe dreapta
			}
		}
		
		if (((float)-(*aa)->a) / (*aa)->b != (float)(-a) / b) {		//cazul in care pantele nu sunt egale(dreptele se intersecteaza)
			ConstrArboreDr_2(arb,&(*aa)->st,a,b,c);		//mergem si pe stanga si pe dreapta pentru a construii frunza
			ConstrArboreDr_2(arb,&(*aa)->dr,a,b,c);		
		}
		
		return arb;
	}

TArb ConstrArborePct(TArb arb,TArb *aa,int x,int y,int ind)
	{
		if (!(*aa)) {
			(*aa)=ConstrFrunza(x,y,ind,1);	//daca se ajunge la un element NULL,aici se va construii noua frunza
			return arb;		
		}

		if ((*aa)->a * x + (*aa)->b * y + (*aa)->c < 0) {
			ConstrArborePct(arb,&(*aa)->st,x,y,ind);	
		//daca ecuatia a*x+b*y+c e mai mica decat 0,inseamna ca punctul(regiunea) se afla in stanga dreptei
		}		

		if ((*aa)->a * x + (*aa)->b * y + (*aa)->c > 0) {
			ConstrArborePct(arb,&(*aa)->dr,x,y,ind);	
		//daca ecuatia a*x+b*y+c e mai mare decat 0,inseamna ca punctul(regiunea) se afla in dreapta dreptei
		}		
	
	return arb;
	}
	
void SDR(TArb arb,FILE *fw)
	{	
		if(!arb) {
			fprintf(fw,"null");	//daca nu exista punct(regiune) asociat unei drepte,afisam null
			return ;
		}	

		if(!arb->st && !arb->dr && arb->type == 1) {
			fprintf(fw,"((%d,%d)-%d)", arb->a,arb->b,arb->c);	
		//daca elementul nu are fii si este de tip 1,inseamna ca este punct(regiune) si-l afisam conform cerintei
			return ;
		}

		SDR(arb->st,fw);	//parcurgem arborul in postordine(Stanga-Dreapta-Radacina)
		SDR(arb->dr,fw);

		fprintf(fw,"(%d,%d,%d)", arb->a,arb->b,arb->c);	//Radacina unui element este intotdeauna o dreapta careia ii afisam coeficientii
		return;
	}

void RegiuniPuncte(TArb arb,TArb *aa,int x,int y,FILE *fw)
	{
		if (!(*aa)->st && !(*aa)->dr && (*aa)->type == 1) {		
			fprintf(fw,"%d\n",(*aa)->c);		
		//daca nodul nu are fii si este de tipul 1,inseamna ca este frunza si deci am gasit regiunea aferenta punctului(afisam indicele acesteia)
			return;
		}
	
		if ((*aa)->a * x + (*aa)->b * y + (*aa)->c < 0){
			RegiuniPuncte(arb,&(*aa)->st,x,y,fw);	//parcurgem arborele in stanga,pana cand gasim regiunea aferenta punctului
		}

		if ((*aa)->a * x + (*aa)->b * y + (*aa)->c > 0) {
			RegiuniPuncte(arb,&(*aa)->dr,x,y,fw);	//parcurgem arborele in dreapta,pana cand gasim regiunea aferenta punctului
		}
	}

void DistrNod(TArb arb) 	
	{ 
		if (!arb) return;
		
		DistrNod (arb->st);     //distruge recursiv toti fii stangi
		DistrNod (arb->dr);    //distruge recursiv toti fii drepti
  
		free (arb);             //elibereaza memoria
	}

void DistrArb(TArb *aa) 
	{ 
		if (!(*aa)) return;   

	 	DistrNod (*aa);   //distrugem arborele   

		*aa = NULL;        //arborele devine NULL      
	}


int main(int argc,char *argv[])
	{
		int mod=0,N=0,i=0,M=0,Q=0,j=0,dreapta[100][3];	
		int x[100],y[100],xq[1000],yq[1000];
		FILE *fr=NULL,*fw=NULL;
		TArb arb=NULL;
		
		mod=atoi(argv[1]);		//modul de implementare(1 sau 2)
		fr=fopen(argv[2],"r");		//deschidem fisierul de citire
		fw=fopen(argv[3],"w");		//deschidem fisierul de scriere
	
		fscanf(fr,"%d",&N);	//numarul de drepte
		for (i=0;i<N;i++) {
			for (j=0;j<3;j++) 
				fscanf(fr,"%d",&dreapta[i][j]);	//memoram intr-o matrice coeficientii fiecarei drepte				
		}
			
		fscanf(fr,"%d",&M);	//numarul de puncte(regiuni)
		for (i=0;i<M;i++) {
			fscanf(fr,"%d",&x[i]);	//memoram intr-un vector toate coordonatele x
			fscanf(fr,"%d",&y[i]);	//memoram intr-un vector toate coordonatele y
		}
	
		if (mod == 1) {	
			ConstrArboreDr_1(arb,&arb,fr,dreapta);	//apelam functia de construire a arborelui facand citirea in functie(primul mod)
		}

		if (mod == 2) {
			for (i=0;i<N;i++) {
				ConstrArboreDr_2(arb,&arb,dreapta[i][0],dreapta[i][1],dreapta[i][2]);
		//pentru fiecare dreapta,apelam functia de construire a arborelui(modul 2)
			}
		}
		
		for(i=0;i<M;i++)
			arb=ConstrArborePct(arb,&arb,x[i],y[i],i);	//introducem in arbore punctele(regiunile)
		
		SDR(arb,fw);	//afisam arborele in postordine(Stanga-Dreapta-Radacina)
		fprintf(fw,"\n");

		fscanf(fr,"%d",&Q);	//numarul de puncte ce vor trebuii gasite
		for (i=0;i<Q;i++) {
			fscanf(fr,"%d",&xq[i]);		//memoram coordonatele x ale punctelor de gasit intr un vector
			fscanf(fr,"%d",&yq[i]);		//memoram coordonatele y ale punctelor de gasit intr un vector
		}
	
		for (i=0;i<Q;i++) {
			RegiuniPuncte(arb,&arb,xq[i],yq[i],fw);		//pentru fiecare punct,cautam si afisam regiunea
		}	

		fclose(fr);
		fclose(fw);

		DistrArb(&arb);		//eliberam memoria ocupata de arbore

		return 0;
	}
	

