%{
#include <stdio.h>
%}



punc ","|"\("|"\)"|"{"|"}"
keyword "while"|"if"|"else"|"int"|"boolean"|"float"
mulop "*"|"/"
letter [a-zA-Z]
digit [0-9]
digits {digit}+
id {letter}({letter}|{digit})*
num {digit}+"."{digits}([eE][-+]?{digit}+)?
relop "<"|">"|"<="|">="|"=="|"!="
assign "="  
addop "+"|"-"



%%
{keyword}       { printf("%s\n", yytext); }
{punc}      { printf("%s\n", yytext); }
{mulop}     { printf("mulop\n"); }
{id}       { printf("ID\n"); }
{digits}    { printf("digits\n"); }
{num}       { printf("NUM\n"); }
{relop}     { printf("relop\n"); }
{assign}    { printf("assign\n"); }
{addop}     { printf("addop\n"); }
";"            { printf(";\n"); }




%%

int yywrap(){
    return 1;
}
int main() {
    yylex();
    return 0;
}
