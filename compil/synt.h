#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct{
	int code;
	
}erreur;


/* CODES ERREUR

   1 ------------> DIVISION
   
   */
erreur E;

typedef struct{
	
	char prem[5];
	char deux[20];
	char trois[20];
	char quatre[20];
}quad;








int cpt=0;

void insert(int code){
	

	E.code=code;
	cpt++;
}




int check_division(int dollar){
	if (  (E.code==1)&&(dollar==0)  ){
		printf("\n\nerreur semantique: Division par 0, ");
		E.code=0;
		return 0;
	}
	
	return -1;
	
	
}

void AnnZero(){
	
	E.code=0;
	
}























