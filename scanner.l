%{
    int lineNumber = 1;
    int running = 1;
    int token;
#include "hash.c"
#include "tokens.h"

int getLineNumber() {
    return lineNumber;
}
    
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

[a-zA-Z_0-9]+    { hashInsert(2, yytext); return(TK_IDENTIFIER); }

"//"+         { BEGIN(SLCOMMENT); }
"///"        { BEGIN(MLCOMMENT); }

"'"[a-zA-Z0-9]"'"        { hashInsert(3, yytext); return LIT_CHAR; }
    
[0-9]+"."[0-9]+           { hashInsert(4, yytext); return LIT_REAL; }


\"(\\.|[^\"\n])*\" { hashInsert(5, yytext); return LIT_STRING; }
" "          {}
"\n"         { ++lineNumber; }
    
 <SLCOMMENT>"\n"     { ++lineNumber; BEGIN(INITIAL); }
 <SLCOMMENT>.
 
 <MLCOMMENT>"\n"     { ++lineNumber; }
 <MLCOMMENT>"\\\\\\"   { BEGIN(INITIAL); }
 <MLCOMMENT>.
%%
    

#include "main.c"


int yywrap() {
    printf("terminando execução\n");
    running = 0;
    return 1;
}

