%{
    int lineNumber = 1;
    int running = 1;
#include "hash.h"
#include "tokens.h"
%}

%x MLCOMMENT
%x SLCOMMENT

%%
    
    "char"                   { return KW_CHAR }
    "int"                    { return KW_INT; }
    "float"                  { return KW_FLOAT; }
    
    "code"                   { return KW_CODE; }
    
    "if"                     { return KW_IF; }
    "else"                   { return KW_ELSE; }
    "while"                  { return KW_WHILE; }
    "input"                  { return KW_INPUT; }
    "print"                  { return KW_PRINT; }
    "return"                 { return KW_RETURN; }
    
    [,;()[]{}=+-*/%<>&|~]    { return yytext[0]; }
    
    "<="                     { return OPERATOR_LE; }
    ">="                     { return OPERATOR_GE; }
    "=="                     { return OPERATOR_EQ; }
    "!="                     { return OPERATOR_DIF; }
	
    [a-zA-Z_0-9]+            { return TK_IDENTIFIER; }
    
    [0-9]+                   { return LIT_INT; }
    
    "//"                     { BEGIN(SLCOMMENT); }
    "///"                    { BEGIN(MLCOMMENT); }
    
    "\n"                     { ++lineNumber; }
    .                        { return TOKEN_ERROR; }
    
    <SLCOMMENT>"\n"            { ++lineNumber; BEGIN(INITIAL); }
    <SLCOMMENT>.
    
    <MLCOMMENT>"\n"            { ++lineNumber; }
    <MLCOMMENT>"\\\"           { BEGIN(INITIAL); }
    <MLCOMMENT>.
    
%%
