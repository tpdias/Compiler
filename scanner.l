%{
    //Thiago Parisotto Dias
    int lineNumber = 1;
    int running = 1;
    int token;
    
#include <stdio.h>
#include "y.tab.h"
#include "hash.c"
int yyparse();


int getLineNumber();
int isRunning();
void initMe();

%}

%x MLCOMMENT
%x SLCOMMENT

%%
"char"       { return KW_CHAR; }
"int"        { return KW_INT; }
"float"      { return KW_FLOAT; }

"code"       { return KW_CODE; }

"if"         { return KW_IF; }
"else"       { return KW_ELSE; }
"while"      { return KW_WHILE; }
"input"      { return KW_INPUT; }
"print"      { return KW_PRINT; }
"return"     { return KW_RETURN; }

[0-9]+       { hashInsert(1, yytext); return LIT_INT; }

"<="    { return OPERATOR_LE; }
">="    { return OPERATOR_GE; }
"=="    { return OPERATOR_EQ; }
"!="    { return OPERATOR_DIF; }

[a-zA-Z_0-9]+    { return(TK_IDENTIFIER); }

"//"+         { BEGIN(SLCOMMENT); }
"///"        { BEGIN(MLCOMMENT); }

"'"[a-zA-Z0-9]"'"        { hashInsert(2, yytext); return LIT_CHAR; }
    
[0-9]+"."[0-9]+           { hashInsert(3, yytext); return LIT_FLOAT; }

[,;()[\]{}=+\-*/%<>&|~] { return yytext[0]; }

\"(\\.|[^\"\n])*\" { hashInsert(4, yytext); return LIT_STRING; }

" "
[\t]
"\n"         { ++lineNumber; }
.         { return TOKEN_ERROR; }

 <SLCOMMENT>"\n"     { ++lineNumber; BEGIN(INITIAL); }
 <SLCOMMENT>.
 
 <MLCOMMENT>"\n"     { ++lineNumber; }
 <MLCOMMENT>"\\\\\\"   { BEGIN(INITIAL); }
 <MLCOMMENT>.
%%
    


int getLineNumber(void) {
    return lineNumber;
}
int isRunning(void) {
    return running;
}
void initMe(void) {
    hashInit();
}
    
#include "main.c"


int yywrap() {
    running = 0;
    return 1;
}


