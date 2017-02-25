#include "HashTable.h"
#define MAXLINE 1000


int hashf(char *KEY,int M)
{
int i=0,l=0,res=0,sum=0;
l=strlen(KEY);
for (i=0;i<l;i++)
	sum=sum+KEY[i];
res=sum%M;

return res;
}

THash *InitHash(int M,TFHASH hashf)
{
THash *r=NULL;
int i=0;
r=(THash*)malloc(sizeof(THash));
if(!r) return NULL;
r->v=(TLG*)calloc(M,sizeof(TLG));
for(i=0;i<M;i++)
	{
	
	r->v[i]=NULL;
	}
if(!r->v)
	{
	free(r);
	return NULL;
	}
r->M=M;
r->hashf=hashf;
return r;
}

TLG ConstrCelula(char *KEY,char *VALUE)
{
	TLG aux=NULL;
	aux=(TLG)malloc(sizeof(TCelulaG));
	if (!aux) {
		return NULL;
	}
	aux->urm=NULL;
	aux->info=(TDNS*)malloc(sizeof(TDNS));
	if(!aux->info)
		{
		free(aux);
		return NULL;
		}
	aux->info->KEY=malloc(100);
	if(!aux->info->KEY)
		{
		free(aux->info);
		free(aux);
		return NULL;
		}
	aux->info->VALUE=malloc(100);
	if(!aux->info->VALUE)
		{
		free(aux->info);
		free(aux);
		return NULL;
		}
	strcpy(aux->info->KEY,KEY);
	strcpy(aux->info->VALUE,VALUE);	
	return aux;
	
}






void put(char *KEY,char *VALUE,THash *r,int M)
{
int k=0;
TLG aux=NULL,ante=NULL;
aux=ConstrCelula(KEY,VALUE);
k=hashf(KEY,M);
TLG p=r->v[k];

if (!p)
	{	
	r->v[k]=aux;		
	}	
	else
		{
		for(;p;ante=p,p=p->urm)
			{			
			if (strcmp(p->info->KEY,aux->info->KEY)==0) 
				{
				free(aux->info->KEY);
				free(aux->info->VALUE);
				free(aux->info);
				free(aux);
				return ;
				}			
				else
				{			
					if(strcmp(p->info->KEY,aux->info->KEY)>0)
					{
						if (p==r->v[k])
						{					
						r->v[k]=aux;
						aux->urm=p;
						return;
						}
					else				
					{
					ante->urm=aux;
					aux->urm=p;
					return ;
					}				
					}
				}		
			}
			if (!p)
				{				
				ante->urm=aux;
				return ;
				}
		}
}




void print(THash *r,int M,FILE *fw)
{
int i=0;
for (i=0;i<M;i++)
	{	
	TLG p=r->v[i];	
	if (p)	
	{
		fprintf(fw,"%d: ",i);
			for(;p;p=p->urm)
  				{
				fprintf(fw,"%s ",p->info->VALUE);
				}
			fprintf(fw,"\n");	
	}
	}
}

void print_bucket(THash *r,int k,FILE *fw)
{
TLG p=r->v[k];
if (!p) 
	{	
	fprintf(fw,"VIDA\n");
	return ;
	}
	else
{
for(;p;p=p->urm)
  		{
		fprintf(fw,"%s ",p->info->VALUE);
		}
	fprintf(fw,"\n");
	return ;	
}
}

char *get(THash *r,char* KEY,int M)
	{
	int k=0;
	k=hashf(KEY,M);
	TLG p=r->v[k];
	for(;p;p=p->urm)
		{
		if (strcmp(p->info->KEY,KEY)==0)
			return p->info->VALUE;			
		}
	return NULL;
	}

int find(THash *r,char* KEY,int M)
	{
	int k=0;
	k=hashf(KEY,M);
	TLG p=r->v[k];
	for(;p;p=p->urm)
		{
		if (strcmp(p->info->KEY,KEY)==0)
			return 1;			
		}
	return 0;
	} 


void Remove(THash *r,int M,char* KEY)
	{
	int k=0;
	k=hashf(KEY,M);
	TLG p=r->v[k],ant=NULL;
	for (;p;ant=p,p=p->urm)
		{		
		if (strcmp(p->info->KEY,KEY)==0)
			{
			if(p==r->v[k])
				{
				r->v[k]=p->urm; //inceput
				free(p->info->KEY);
				free(p->info->VALUE);
				free(p->info);
				free(p);
					return;
				}
			else
				if(p->urm==NULL)
				{
				ant->urm=NULL; //sfarsit
			free(p->info->KEY);
				free(p->info->VALUE);
				free(p->info);
				free(p);
					return;
				}			
				else
				{
				ant->urm=p->urm; //mijloc
				free(p->info->KEY);
				free(p->info->VALUE);
				free(p->info);
				free(p);
					return;
				}			
						
			}
		}
	}

void DistrugeLG(ALG av)
	{
	TLG aux=NULL,p=NULL;
	p=*av;

	while(p)
	{
		aux=p;
		p=p->urm;
		free(aux->info->VALUE);
		free(aux->info->KEY);
		free(aux->info);		
		free(aux);
		
	}
	
	*av=NULL;
	}

void DistrugeTHash(THash **r,int M)
{
	int k=0;
	TLG p=NULL;
	for(k=0;k<M;k++)
		{
			p=(*r)->v[k];
			DistrugeLG(&p);
			
		}
		free((*r)->v);		
		free(*r);
		*r=NULL;
}




int main(int argc,char *argv[])
{
int M=0,k,res=0;
char *line=NULL,*word=NULL,*KEY=NULL,*VALUE=NULL,*afis=NULL;
FILE *fr=NULL,*fw=NULL;
THash *r=NULL;
M=atoi(argv[1]);
r=InitHash(M,hashf);
fr=fopen(argv[2],"r");
fw=fopen(argv[3],"w");
	line=(char *)malloc(sizeof(char)*MAXLINE);
	if (!line)
		return 0; 

 
   while (fgets(line,100, fr)!=NULL)
    {	
	word =strtok (line," \n");
	while(word!=NULL)
	{
    	if (strcmp(word,"put")==0)
		{			
		KEY=strtok(NULL," \n");
		VALUE=strtok(NULL," \n");			
		put(KEY,VALUE,r,M);		
		}
		if (strcmp(word,"get")==0)
			{
			KEY=strtok(NULL," \n");
			afis=get(r,KEY,M);
			if (afis)
			fprintf(fw,"%s\n",afis);
			else
			fprintf(fw,"NULL\n");			
			}
		if (strcmp(word,"remove")==0)
			{
			KEY=strtok(NULL," \n");			
			Remove(r,M,KEY);
			}
		if (strcmp(word,"find")==0)
			{
			KEY=strtok(NULL," \n");			
			res=find(r,KEY,M);			
			if (res==1)
			fprintf(fw,"True\n");
			else
			fprintf(fw,"False\n");			
			}	
		if (strcmp(word,"print")==0)
			print(r,M,fw);	
		if (strcmp(word,"print_bucket")==0)
			{
			k=atoi(strtok(NULL," \n"));
			if(k>=0 && k<M)
			{	
			printf("%d",k);
			print_bucket(r,k,fw);
			}
			}
		word=strtok(NULL," ");
		}
	}

fclose(fr);
fclose(fw);
free(line);
DistrugeTHash(&r,M);

}
