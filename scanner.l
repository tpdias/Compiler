%{
    int lineNumber = 1;
    int running = 1;
#include "hash.h"
#include "tokens.h"
%}



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

%%
