#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int temporaire;
int qcc;

typedef struct{
	
	char prem[50];
	char deux[20];
	char trois[20];
	int quatre;
	char cinq[10]
}quad;

quad* tabQuad;

typedef struct{
	int position;
	struct Epos* next ;
	
}Epos;

Epos* pile;


typedef struct{
	char operande[9];
	struct Earith* next ;
	
}Earith;

Earith* PileArith;


typedef struct{
	int state;
	char name[9];
	char nature[20];
}TSK;

typedef struct{
	int state;
	char name[8];
	char nature[21];
}TSS;


typedef struct TSI{
	char name[9];
	char nature[20];
	char type[8];
	float Fval;
	int   Ival;
	char  Cval;
	char* Sval;
	int taille;
	int d;
	int t;
	struct TSI* next;
}TSI;

typedef struct TSC{
	char name[50];
	char nature[20];
	char type[8];
	float Fval;
	int   Ival;
	char  Cval;
	char* Sval;
	struct TSC* next;
}TSC;


typedef struct{
	TSK keys[17];
	TSI* idfs;
	TSC* constants;
	TSS symbols[40];

}TS;


typedef struct{
	
	TSI* idf;
	
}PointeurTSI;






TS TABLE;
TSI* idf;
int PremTSI;
PointeurTSI PTSI[100];

TSC* cons;
int ERROR;

void afficher(){
	TSC* p;
	TSI* c;
	int i;
	
	printf("\n\n");
	printf(" 		KEYS		\n\n");
	printf("   state	name	   nature\n\n");
	
	for(i=0;TABLE.keys[i].state==1;i++){
	printf("   %d            %s         %s\n",TABLE.keys[i].state,TABLE.keys[i].name,TABLE.keys[i].nature);
	}
	
	printf("\n\n");
	
	
	printf(" 		symbols		\n\n");
	printf("   state   name	   nature\n\n");
	
	for(i=0;TABLE.symbols[i].state==1;i++){
		printf("   %d       %s        %s\n",TABLE.symbols[i].state,TABLE.symbols[i].name,TABLE.symbols[i].nature);
	}
	
	printf("\n\n");
	
	printf(" 		Constantes		\n\n");
	printf(" name        nature       type     value   \n\n");
	
	for(p=TABLE.constants; p!=NULL;p=p->next){
		
		
		if(strcmp(p->type,"FLOAT")==0)
			printf("%s        %s       %s        %f  \n",p->name,p->nature,p->type,p->Fval);
		else{
			if(strcmp(p->type,"INTEGER")==0)
				printf("   %s        %s       %s      %d  \n",p->name,p->nature,p->type,p->Ival);
			else{
				if(strcmp(p->type,"CHAR")==0)
					printf("    %s        %s       %s        %c  \n",p->name,p->nature,p->type,p->Cval);
				else{
					printf("  %s        %s        %s        %s \n",p->name,p->nature,p->type,p->Sval);
				}
			}
			
		}	
	}
	
	printf("\n\n");
	
	printf(" 		IDFS		\n\n");
	printf(" name        nature        type		val	  taille \n\n");
	
	for(c=TABLE.idfs;c!=NULL;c=c->next){
		printf("%s        %s       %s                          ",c->name,c->nature,c->type);
		
		if(c->t==1)
			printf("%d   ",c->taille);
		printf("\n");
		
	}
	
	
}

char* substring(char* string){
	
	char* res;
	int i;
	res=(char*)malloc(strlen(string)-1*sizeof(char));
	
	for(i=0;i<strlen(string)-2;i++){
		res[i]=string[i+1];
	}
	res[i]='\0';
	
	return res;
	}



void insertKey(char* name,int pos){ //mots cles
	TABLE.keys[pos].state=1;
	strcpy(TABLE.keys[pos].name,name);
	strcpy(TABLE.keys[pos].nature,"KEY WORD");


	
}

void insert_symbol(char* name, int pos){  //idf et symboles
	TABLE.symbols[pos].state=1;
	strcpy(TABLE.symbols[pos].name,name);
	strcpy(TABLE.symbols[pos].nature,"symb");


	
}

void insertConst(char* name, TSC* p, int code){ //constantes
	
	char* n;
	
	if(cons==NULL){
		cons=(TSC*)malloc(sizeof(TSC));
		TABLE.constants=cons;
		p=cons;
	}
	else{
		p->next=(TSC*)malloc(sizeof(TSC));
		p=p->next;
	}
	
	p->next=NULL;
	
	strcpy(p->nature, "const");
	
	switch(code){
	case 21:
		if(  (atoi(name)>=-32768) && (atoi(name)<=32767)){
		strcpy(p->name,name);
		strcpy(p->type,"INTEGER");
		p->Ival=atoi(name);
		}
		else{
			printf("%s doit etre entre -32768 et 32767.\n",name);
		}
		break;
	case 22:
		strcpy(p->name,name);
		strcpy(p->type,"FLOAT");
		p->Fval=atof(name);
		break;
	case 23:
		
		n=substring(name);
		strcpy(p->name,n);
		strcpy(p->type,"CHAR");
		p->Cval=p->name[0];
	

		break;
		
	case 24:
		n=substring(name);
		strcpy(p->name,n);
		p->Sval=(char*)malloc(sizeof(p->name));
		strcpy(p->Sval,p->name);
		strcpy(p->type,"STRING");
	
		break;
	}
	


}


void insertIdf(char* name, TSI* p){ //IDF
	
	if(idf==NULL){

		
		idf=(TSI*)malloc(sizeof(TSI));
		TABLE.idfs=idf;
		p=idf;
	}
	else{
		p->next=(TSI*)malloc(sizeof(TSI));
		p=p->next;
	}
	
	p->next=NULL;
	p->d=0;
	p->t=0;
	strcpy(p->name,name);
	strcpy(p->nature, "IDF");
	strcpy(p->type,"1");
	
	
	
	
}

void rechercher(char* name, int code){

	int i;
	TSC *p0=cons;
	TSC *pr0=cons;

	TSI *p=idf;
	TSI *pr=idf;
	
	switch (code){
		case 0: //KEYS
		
			for(i=0; ( (i<16) && (TABLE.keys[i].state==1) && (strcmp(name,TABLE.keys[i].name)!=0)) ;i++);
		
			if((i<16)&&(TABLE.keys[i].state==0)){
			insertKey(name,i);
				}
			
			break;
		case 1: //TABLE.symbols
			for(i=0; ( (i<40) && (TABLE.symbols[i].state==1) && (strcmp(name,TABLE.symbols[i].name)!=0)) ;i++);
		
			if((i<40)&&(TABLE.symbols[i].state==0)){
				insert_symbol(name,i);
				}
				
			
			
			break;
			
		case 21://CONST Entier
		case 22://CONST Reel
		case 23://CONST Character
		case 24://CONST String
			
			
			while( (p0!=NULL) && (strcmp(p0->name,name)!=0) )  {
				
				pr0=p0;
				p0=p0->next;
			}
			
			if(p0==NULL)
				insertConst(name,pr0,code);
			
			break;
		
		case 3: //IDF
			
			if(strlen(name)>8)
				printf("IDF has more than 8 caracters\n");
			else{
				
				while( (p!=NULL) && (strcmp(p->name,name)!=0) )  {
				
					pr=p;
					p=p->next;
				}
			
				if(p==NULL)
					insertIdf(name,pr);
				
			}
			break;
	}
}

int NbEspace(char* s){
	int nb=0;
	int i=0;
	
	for(i=0;i<strlen(s);i++){
		if(s[i]=='\n')
			nb++;
	}
	return nb;
	
}


void init(){
	int i;
	
	PileArith=NULL;
	pile=NULL;
	idf=NULL;
	cons=NULL;
	PremTSI=0;
	temporaire=0;
	
	for (i=0;i<20;i++){
		TABLE.keys[i].state=0;
	}
	
	for (i=0;i<20;i++){
		TABLE.symbols[i].state=0;	
	}
	
	TABLE.constants=cons;
	TABLE.idfs=idf;
	
		
}






int CheckDouble(char* idf){
	
	TSI* c;
	
	for(c=TABLE.idfs; (strcmp(idf,c->name)) ;c=c->next);
	
	if(c->d==0){
		c->d++;

		return 0;
	}
	
	if(c->d==1){
		c->d++;
		printf("erreur semantique: double declaration de %s\n",idf);
		
		return 1;
	}
		
}	
	
	
void MajType(char* type){
	
	
	TSI* c;
	
	for(c=TABLE.idfs; c!=NULL ;c=c->next){
		
	
			if(!strcmp(c->type,"1")){
				
				strcpy(c->type,type);
			}
			
		
		
		
	}
	
	
}

void Tupdate(int taille, char*idf){
	
	
	TSI* c;
	
	for(c=TABLE.idfs; c!=NULL ;c=c->next){
		
	
			if(!strcmp(c->name,idf)){
				c->t=1;
				c->taille=taille;
				
			}
	}
}

void fits(char*tab, int taille){
	
	TSI* c;
	
	for(c=TABLE.idfs; c!=NULL ;c=c->next){
		
	
			if(!strcmp(c->name,tab)&&  (taille>=c->taille)   ){
				
				printf("erreur semantique: depassement taille tableau.");
				
			}
	}
	
	
}









void addQuad(char* prem,char*deux,char*trois,int qc,char*cinq){
	
	
	strcpy(tabQuad[qc].prem,prem);
	strcpy(tabQuad[qc].deux,deux); 
	strcpy(tabQuad[qc].trois,trois); 
	strcpy(tabQuad[qc].cinq,cinq); 
	tabQuad[qc].quatre=qcc;
}


void AffQuad(){
	
	int i;
	
	  printf("\n\n		Liste des quadruplets\n\n");
	for(i=0;i<qcc;i++){
		
		
		
		printf("\" %s \"     \"%s\"	\"%s\"     \"%s\" 	%d\n",tabQuad[i].prem,tabQuad[i].deux,tabQuad[i].trois,tabQuad[i].cinq,tabQuad[i].quatre);
		
		
	}
	
	
}



void EmpilerPos(int pos){
	
	Epos*q;
	q=(Epos*)malloc(sizeof(Epos));
	
	q->position=pos;
	q->next=pile;
	pile=q;
}

int DepilerPos(){
	
	int p=-1;
	Epos* q;
	
	if(pile!=NULL){
	p=pile->position;
	q=pile;
	pile=pile->next;
	
	free(q);
	}
	return p;
	
	
}


void UpdateQuad(int pos){
	
	int p;
	char str[50];
	
	p=DepilerPos();
	if(p!=-1){
	sprintf(str, "%d", pos);
	strcpy(tabQuad[p].deux,str);
	}
	
	
}

void EmpilerOp(char* op){
	
	Earith*q;

	q=(Earith*)malloc(sizeof(Earith));
	
	stpcpy(q->operande,op);
	q->next=PileArith;
	
	PileArith=q;
}

Earith* DepilerOp(){
	
	char* p;
	Earith* q;
	p=(char*)malloc(9*sizeof(char));
	

	stpcpy(p,PileArith->operande);
	q=PileArith;
	PileArith=PileArith->next;
	
	return q;
	
	
}

char* GenTemp(){
	static const char* p;
	char*fg;

	p=(char*)malloc(9*sizeof(char));
	strcpy(p,"t");
	sprintf(fg,"%d",temporaire);
	strcat(p,fg);
	
	temporaire++;
	return p;
	
}

///////////////////////////////////////////////////////////////////////////