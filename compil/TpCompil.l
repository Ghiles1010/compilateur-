%{
#include"TS.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"TpCompilS.tab.h"
extern YYSTYPE yylval;
int line=1;
int col=1;
int yyparse();
int erreurLex=0;

%}
%option noyywrap



Letter [A-Za-z]
Number [0-9]
IDF [A-Z]([A-Za-z]|{Number})*
FloatConst {Number}+(\.{Number}+)?
IntConst {Number}+
Comment [/]{2}([^([\][\])]|\n)*?[/]{2}
Char '.'
STR \".*\"

%%
BEGIN {rechercher(yytext,0); col=col+strlen(yytext); return MBEGIN;}
LET {rechercher(yytext,0); col=col+strlen(yytext);return LET;}
VAR {rechercher(yytext,0); col=col+strlen(yytext);return VAR;}
INTEGER {rechercher(yytext,0); col=col+strlen(yytext);yylval.chaine=strdup(yytext);return INTEGER;}
FLOAT {rechercher(yytext,0); col=col+strlen(yytext);yylval.chaine=strdup(yytext);return FLOAT;}
CHAR {rechercher(yytext,0); col=col+strlen(yytext);yylval.chaine=strdup(yytext);return CHAR;}
STRING {rechercher(yytext,0); col=col+strlen(yytext);yylval.chaine=strdup(yytext);return STRING;}
PROGRAM {rechercher(yytext,0); col=col+strlen(yytext);return PROGRAM;}
GET {rechercher(yytext,0); col=col+strlen(yytext);return GET;}
SHOW {rechercher(yytext,0); col=col+strlen(yytext);return SHOW;}
IF {rechercher(yytext,0); col=col+strlen(yytext);return IF;}
ELSE {rechercher(yytext,0); col=col+strlen(yytext);return ELSE;}
RETURN {rechercher(yytext,0); col=col+strlen(yytext);return RETURN;}
ENDIF {rechercher(yytext,0); col=col+strlen(yytext);return ENDIF;}
FOR {rechercher(yytext,0); col=col+strlen(yytext);return FOR;}
END_FOR {rechercher(yytext,0); col=col+strlen(yytext);return END_FOR;}
END {rechercher(yytext,0); col=col+strlen(yytext); return END;}


{IDF}        {rechercher(yytext,3); col=col+strlen(yytext);yylval.chaine=strdup(yytext); return IDF; }

{IntConst}   {rechercher(yytext,21); col=col+strlen(yytext); yylval.integer=atoi(yytext);return IntConst;}
{FloatConst} {rechercher(yytext,22); col=col+strlen(yytext); yylval.real=atof(yytext);return FloatConst;}
{STR} { rechercher(yytext,24); col=col+strlen(yytext);yylval.chaine=strdup(substring(yytext)); return STR;}
{Char} {rechercher(yytext,23); col=col+strlen(yytext);return Char;}


{Comment} {line+=NbEspace(yytext);}

"||" {rechercher(yytext,1); col=col+strlen(yytext);return DB;}
":" {rechercher(yytext,1); col=col+strlen(yytext);return DP;}
"@" {rechercher(yytext,1); col=col+strlen(yytext);return ADDR;}
"[" {rechercher(yytext,1); col=col+strlen(yytext);return CRO;}
"]" {rechercher(yytext,1); col=col+strlen(yytext);return CRF;}
"{" {rechercher(yytext,1); col=col+strlen(yytext);return AO;}
"}" {rechercher(yytext,1); col=col+strlen(yytext);return AF;}
"+" {rechercher(yytext,1); col=col+strlen(yytext);return PLUS;}
"-" {rechercher(yytext,1); col=col+strlen(yytext);return MOINS;}
"*" {rechercher(yytext,1); col=col+strlen(yytext);return MUL;}
"/" {rechercher(yytext,1); col=col+strlen(yytext);return DIV;}
"=" {rechercher(yytext,1); col=col+strlen(yytext);return AFF;}
"(" {rechercher(yytext,1); col=col+strlen(yytext);return PO;}
")" {rechercher(yytext,1); col=col+strlen(yytext);return PF;}
">>" {rechercher(yytext,1); col=col+strlen(yytext);return SUP;}
"<<" {rechercher(yytext,1); col=col+strlen(yytext);return INF;}
">>=" {rechercher(yytext,1); col=col+strlen(yytext);return SUPE;}
"<<=" {rechercher(yytext,1); col=col+strlen(yytext);return INFE;}
"==" {rechercher(yytext,1); col=col+strlen(yytext);return EGALE;}
"<<>>" {rechercher(yytext,1); col=col+strlen(yytext);return DIFF;}
"£" {rechercher(yytext,1); col=col+strlen(yytext);return FINST;}



" " col++;
"\t" col=col+8;


\n {line++; col=1;}


. {printf("lexical Error in the line %d colone %d \n", line,col);erreurLex=1;}

%%

int main(){

	system("cls");
	init();
	
	yyin=fopen("input.txt","r");
	
	if(yyin==NULL)
		printf("Error when reading the input file");
	else{
		tabQuad=(quad*)malloc(sizeof(quad)*100);
		qcc=0;
		yyparse();
		afficher();
		AffQuad();
		}
		
	
	return 0;
}



