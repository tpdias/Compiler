%{
    // Thiago Parisotto Dias
    #include <stdio.h>
    #include <stdlib.h>
    
    int yylex();
    int yyerror();
    extern int getLineNumber();
    
%}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT

%token KW_CODE

%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_INPUT
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token TK_IDENTIFIER
%token LIT_INT
%token LIT_FLOAT // aqui tem que ser lit_real??
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

%left OPERATOR_OR OPERATOR_AND OPERATOR_NOT
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%start program


%%
    
    program: ldec lcode
    ;
    
    ldec: dec ldec
    |
    ;
    
    dec: type TK_IDENTIFIER '=' lit ';'
    | vetdec
    | fundec
    ;
    
    
    vetdec: type TK_IDENTIFIER '[' LIT_INT ']' lvec ';'
    ;
    
    
    lvec: lit lvec
    |
    ;
    
    type: KW_INT
    | KW_CHAR
    | KW_FLOAT
    ;
    
    lit: LIT_INT
    | LIT_CHAR
    | LIT_FLOAT
    ;
    
    fundec: type TK_IDENTIFIER '(' parameters ')' ';'
    ;
    
    parameters: 
    | param lastparam
    ;
    
    lastparam: 
    | ',' param lastparam
    ;

    param: type TK_IDENTIFIER
    ;

    lcode: KW_CODE TK_IDENTIFIER block lcode
    |
    ;
    
    block: '{' lcmd '}'
    ;
    
    lcmd: cmd lcmd
    |
    ;
    
    cmd: block
    | attr ';'
    | ctrlflx
    | KW_PRINT LIT_STRING ';'
    | KW_PRINT expr ';'
    | KW_RETURN expr ';'
    | ';'
    ;
    
    attr: TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    ;
    
    expr : TK_IDENTIFIER
        | TK_IDENTIFIER '[' expr ']'
        | TK_IDENTIFIER '(' l_args ')'
        | lit
        | expr '+' expr
        | expr '-' expr
        | expr '*' expr
        | expr '/' expr
        | expr '<' expr
        | expr '>' expr
        | expr OPERATOR_LE expr
        | expr OPERATOR_GE expr
        | expr OPERATOR_EQ expr
        | expr OPERATOR_DIF expr
        | expr OPERATOR_AND expr
        | expr OPERATOR_OR expr
        | OPERATOR_NOT expr
        | '(' expr ')'
        | KW_INPUT '(' type ')'
        ;
        
    l_args :
           | expr l_args_end
           ;

    l_args_end :
       | ',' expr l_args_end
       ;
       
    ctrlflx: KW_IF '(' expr ')' cmd
       | KW_IF '(' expr ')' cmd KW_ELSE cmd
       | KW_WHILE '(' expr ')' cmd
       ;
    
%%
    
    
int yyerror() {
    fprintf(stderr, "Erro na linha %d\n", getLineNumber());
    exit(3);
}
