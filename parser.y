%{
// Thiago Parisotto Dias
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "hash.h"
#include "semantic.h"
#include "tacs.h"
int yylex();
int yyerror(char* err);
extern int getLineNumber();
extern TAC* codegen(AST *node);
void check_semantic(int semantic_errors);
AST *root;
TAC *genTACs(AST* node);
%}

%union
{
struct hash_node* symbol;
struct ast_node* ast;
}

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

%token <symbol> TK_IDENTIFIER
%token <symbol> LIT_INT
%token <symbol> LIT_FLOAT 
%token <symbol> LIT_CHAR
%token <symbol> LIT_STRING

%token TOKEN_ERROR

%left OPERATOR_OR OPERATOR_AND OPERATOR_NOT
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%type <ast> program
%type <ast> ldec
%type <ast> dec
%type <ast> lvec
%type <ast> type
%type <ast> lit
%type <ast> parameters
%type <ast> lastparam
%type <ast> param
%type <ast> lcode
%type <ast> block
%type <ast> lcmd
%type <ast> cmd
%type <ast> expr
%type <ast> l_args
%type <ast> l_args_end
%type <ast> ldecinit

%start program

%%
    program: ldecinit                                                       {
                                                                                root = $$;
                                                                                astPrint($1,0);
                                                                                check_and_set_declarations(root, root);
                                                                                check_and_set_nodes(root);
                                                                                check_undeclared(root);
                                                                                check_usage(root, root);
                                                                                check_operands(root);
                                                                                check_misc(root);
                                                                                check_semantic(get_total_semantic_errors());
                                                                                tacPrintForward(genTACs(root));                                                        
                                                                                }
    ;

    ldecinit: ldec lcode                                                     {$$ = astCreate(AST_LDECINIT, 0, $1, $2, 0, 0);}
    ;
    
    ldec: dec ldec                                                          {$$=astCreate(AST_LDEC, 0, $1, $2, 0, 0);}
    |                                                                       {$$=0;}
    ;
    
    dec: type TK_IDENTIFIER '=' lit ';'                                     {$$=astCreate(AST_DECVAR, $2, $1, $4, 0, 0);}
    | type TK_IDENTIFIER '[' LIT_INT ']' lvec ';'                           {$$=astCreate(AST_DECVEC, $2, $1, astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), $6, 0);}                                                                
    | type TK_IDENTIFIER '(' parameters ')' ';'                             {$$=astCreate(AST_DECFUNC, $2, $1, $4, 0, 0);}                                                                
    ;
    
    lvec: lit lvec                                                       {$$=astCreate(AST_VECLST, 0, $1, $2, 0, 0);}
    |                                                                       {$$=0;}
    ;

    
    type: KW_INT                                                            {$$=astCreate(AST_TYPEINT, 0, 0, 0, 0, 0);}
    | KW_CHAR                                                               {$$=astCreate(AST_TYPECHAR, 0, 0, 0, 0, 0);}
    | KW_FLOAT                                                              {$$=astCreate(AST_TYPEFLOAT, 0, 0, 0, 0, 0);}
    ;
    
    lit: LIT_INT                                                            {$$=astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    | LIT_CHAR                                                              {$$=astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    | LIT_FLOAT                                                             {$$=astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    ;

    parameters:                                                             {$$=0;}
    | param lastparam                                                       {$$=astCreate(AST_PARAMINIT, 0, $1, $2, 0, 0);}
    ;
    
    lastparam:                                                              {$$=0;}
    | ',' param lastparam                                                   {$$=astCreate(AST_PARAMLST, 0, $2, $3, 0, 0);}
    ;

    param: type TK_IDENTIFIER                                               {$$=astCreate(AST_PARAM, $2, $1, 0, 0, 0);}
    ;

    lcode: KW_CODE TK_IDENTIFIER block lcode                                {$$=astCreate(AST_LCODE, $2, $3, $4, 0, 0);}
    |                                                                       {$$=0;}
    ;
    
    block: '{' lcmd '}'                                                     {$$=astCreate(AST_BLOCK, 0, $2, 0, 0, 0);}
    ;
    
    lcmd: cmd lcmd                                                          {$$=astCreate(AST_CMDLST, 0, $1, $2, 0, 0);}
    |                                                                       {$$=0;}
    ;
    
    cmd: block                                                              {$$=$1;}
    | TK_IDENTIFIER '=' expr                                                {$$=astCreate(AST_ATTREXPR, $1, $3, 0, 0, 0);}
    | TK_IDENTIFIER '[' expr ']' '=' expr                                   {$$=astCreate(AST_ATTRVEC, $1, $3, $6, 0, 0);}
    | KW_PRINT LIT_STRING ';'                                               {$$=astCreate(AST_PRINT, 0, astCreate(AST_SYMBOL, $2, 0, 0, 0 ,0), 0, 0, 0);}                
    | KW_PRINT expr ';'                                                     {$$=astCreate(AST_PRINTEXP, 0, $2, 0, 0, 0);}        
    | KW_RETURN expr ';'                                                    {$$=astCreate(AST_RETURN, 0, $2, 0, 0, 0);}            
    | KW_IF '(' expr ')' cmd                                                {$$=astCreate(AST_IF, 0, $3, $5, 0, 0);}
    | KW_IF '(' expr ')' cmd KW_ELSE cmd                                    {$$=astCreate(AST_IF, 0, $3, $5, $7, 0);}
    | KW_WHILE '(' expr ')' cmd                                             {$$=astCreate(AST_WHILE, 0, $3, $5, 0, 0);}
    | ';'                                                                   {$$=0;}
    ;
    
    expr : TK_IDENTIFIER                                                    {$$=astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | TK_IDENTIFIER '[' expr ']'                                        {$$=astCreate(AST_VEC, $1, $3, 0, 0, 0);}
        | TK_IDENTIFIER '(' l_args ')'                                      {$$=astCreate(AST_FUNC, $1, $3, 0, 0, 0);}
        | lit                                                               {$$=$1;}
        | expr '+' expr                                                     {$$=astCreate(AST_ADD, 0, $1, $3, 0, 0);}
        | expr '-' expr                                                     {$$=astCreate(AST_SUB, 0, $1, $3, 0, 0);}
        | expr '*' expr                                                     {$$=astCreate(AST_MUL, 0, $1, $3, 0, 0);}
        | expr '/' expr                                                     {$$=astCreate(AST_DIV, 0, $1, $3, 0, 0);}
        | expr '<' expr                                                     {$$=astCreate(AST_LESS, 0, $1, $3, 0, 0);}
        | expr '>' expr                                                     {$$=astCreate(AST_GREATER, 0, $1, $3, 0, 0);}
        | expr OPERATOR_LE expr                                             {$$=astCreate(AST_LE, 0, $1, $3, 0, 0);}                            
        | expr OPERATOR_GE expr                                             {$$=astCreate(AST_GE, 0, $1, $3, 0, 0);}                                                
        | expr OPERATOR_EQ expr                                             {$$=astCreate(AST_EQ, 0, $1, $3, 0, 0);}                                                
        | expr OPERATOR_DIF expr                                            {$$=astCreate(AST_DIF, 0, $1, $3, 0, 0);}                                                                        
        | expr OPERATOR_AND expr                                            {$$=astCreate(AST_AND, 0, $1, $3, 0, 0);}                                                                        
        | expr OPERATOR_OR expr                                             {$$=astCreate(AST_OR, 0, $1, $3, 0, 0);}                                                
        | OPERATOR_NOT expr                                                 {$$=astCreate(AST_NOT, 0, $2, 0, 0, 0);}                        
        | '(' expr ')'                                                      {$$=astCreate(AST_PARENTESES, 0, $2, 0, 0, 0);}
        | KW_INPUT '(' type ')'                                             {$$=astCreate(AST_INPUT, 0, $3, 0, 0 ,0);}        
        ;
        
    l_args :                                                                {$$=0;}
           | expr l_args_end                                                {$$=astCreate(AST_ARGLST, 0, $1, $2, 0, 0);}
           ;

    l_args_end :                                                            {$$=0;}
       | ',' expr l_args_end                                                {$$=astCreate(AST_ARGLSTEND, 0, $2, $3, 0, 0);}
       ;
       
    
    
%%
    
AST *getRoot() {
    return root;
}

void check_semantic(int semantic_errors) {
    if(semantic_errors > 0) {
        fprintf(stderr, "%d, Semantic Errors.\n", semantic_errors);
        exit(4);
    }
}

int yyerror(char* err) {
    fprintf(stderr, "Erro na linha %d\n", getLineNumber());
    exit(3);
}

TAC *genTACs(AST* node) {
    return revertTac(codegen(node));
}
