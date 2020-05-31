%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"synt.h"

extern FILE* yyin ;
extern int line;
extern int col;
int yylex();
int yyerror(char* s);
int NbrVar=1;
int qcc;
char*b;
char*c;
char*d;
char buf[20];
char NomTab[20];
%}
%union
{
	float real;
	char* chaine;
	int integer;
	char car;
}
%left PLUS MOINS
%left MUL DIV
%right PO PF

%token MBEGIN LET VAR INTEGER FLOAT STRING CHAR STR PROGRAM 
%token GET SHOW FOR END_FOR IF ENDIF ELSE ENDIF RETURN END

%token <chaine>IDF

%token IntConst <real>FloatConst <car>Char 
%token STRING

%token DB DP ADDR CRO CRF AO AF 
%token AFF SUP INF SUPE INFE EGALE DIFF FINST

%type <integer> IntConst
%type <chaine> CHAR
%type <chaine> STRING
%type <chaine> FLOAT
%type <chaine> INTEGER
%type <chaine> PLUS


%start S  
%%

S: entete declaration MBEGIN corps END{ printf ("Analyse syntaxique terminee sans erreurs"); YYACCEPT;}
;




entete: PROGRAM IDF 
;



 
declaration: VAR idfTABtrv dec2 | LET IDF type AFF valeur FINST dec1 
;

dec2: CRO tailletrv CRF DP CRO type CRF FINST dec1 | listevar DP type FINST dec1 
;

tailletrv: IntConst {Tupdate($1,NomTab); }
;

idftrv: IDF {  CheckDouble($1); }
;

idfTABtrv: IDF { strcpy(NomTab,$1); CheckDouble($1); }
;

dec1: /*vide*/ | declaration  
;

listevar: /*vide*/ | DB idftrv listevar 
;

type: INTEGER {MajType($1);}| FLOAT {MajType($1);}| CHAR {MajType($1);}| STRING {MajType($1);}
;

valeur: FloatConst | IntConst | Char | STR
;

corps: /*vide*/ | instruction POUND corps | IFcondition corps | FORcondition corps
;

POUND: FINST
;

instruction: NTAB IsTAB AFF affectation {b=DepilerOp(); c=DepilerOp(); addQuad("="," ",b,qcc,c);qcc++} | ES
;

NTAB: IDF {strcpy(NomTab,$1); EmpilerOp($1); }
;

IsTAB: /*vide*/ | CRO IntConst CRF  {fits(NomTab,$2);printf("ligne %d , colonne %d\n",line,col-1);}
;

/*************************** ARITHMETIQUE**************************************************/
affectation: exprArithm
;

exprArithm: zaid
;

zaid: zaid PLUS nakis {b=DepilerOp(); c=DepilerOp(); d=GenTemp(); addQuad("+",b,c,qcc,d); qcc++; EmpilerOp(d);} | nakis
;

nakis: nakis MOINS fi {b=DepilerOp(); c=DepilerOp(); d=GenTemp(); addQuad("-",b,c,qcc,d); qcc++; EmpilerOp(d);}| fi
;


fi: fi MUL kis {b=DepilerOp(); c=DepilerOp(); d=GenTemp(); addQuad("*",b,c,qcc,d); qcc++; EmpilerOp(d);}| kis
;

kis: kis division others {b=DepilerOp(); c=DepilerOp(); d=GenTemp(); addQuad("/",b,c,qcc,d); qcc++; EmpilerOp(d);}| others
;

division: DIV {insert(1);}
;

others: MOINS others | br
;

br: parouvr zaid PF  | operande
;

parouvr: PO {AnnZero();}
;

operande: NT IsTAB | num 
;

NT: IDF {EmpilerOp($1);}
;

num: FloatConst  {sprintf(buf, "%f", $1); EmpilerOp(buf);}| IntConst {if(!check_division($1)) {printf("ligne %d , colonne %d\n",line,col-1);}  sprintf(buf, "%d", $1); EmpilerOp(buf);}
;
/**********************************ENTREE SORTIE*******************************************/

ES: GET PO STR DP ADDR IDF PF | SHOW PO STR DP ADDR IDF PF

;

/**********************************CONDITIONS*****************************/

IFcondition: IF PO expression PF AO corps RETURN PO IDF PF AF autre
;

autre: ENDIF {UpdateQuad(qcc);}| EL DP AO corps RETURN PO IDF PF AF ENDIF
;

EL: ELSE {UpdateQuad(qcc); }
;


expression: affectation Sexpr {addQuad("BZ","FIN "," ",qcc," "); EmpilerPos(qcc); qcc++;}
;

Sexpr: /*vide*/ | comparateurs affectation

comparateurs: SUP | SUPE | INF | INFE | EGALE | DIFF
;

FORcondition: FOR PO IDF DP num DP operande comparateurs operande PF corps END_FOR
;







%%
int yyerror(char *msg)
{

printf("%s ligne %d , colonne %d\n",msg,line,col-1);

return 1;
}




int yywrap(void)
{
}