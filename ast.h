/* Thiago Parisotto Dias */
#ifndef AST_HEADER 
#define AST_HEADER

#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SONS 4

#define AST_SYMBOL 0
#define AST_ADD 1
#define AST_SUB 2
#define AST_MUL 3
#define AST_DIV 4
#define AST_LESS 5
#define AST_GREATER 6
#define AST_LE 7
#define AST_GE 8
#define AST_EQ 9
#define AST_DIF 10
#define AST_AND 11
#define AST_OR 12
#define AST_NOT 13
#define AST_LDEC 14
#define AST_DECVAR 15
#define AST_DECVEC 16
#define AST_DECFUNC 17
#define AST_PARAM 18
#define AST_BLOCK 19
#define AST_CMDLST 20
#define AST_PRINT 22
#define AST_RETURN 23
#define AST_IF 24
#define AST_ELSE 26
#define AST_WHILE 27
#define AST_INPUT 28
#define AST_CODE 29
#define AST_TYPEINT 30
#define AST_TYPEFLOAT 31
#define AST_TYPECHAR 32
#define AST_VECLST 33
#define AST_PARAMINIT 34
#define AST_PARAMLST 35
#define AST_LCODE 36
#define AST_ATTR 37
#define AST_ATTREXPR 38
#define AST_ATTRVEC 39
#define AST_PARENTESES 40
#define AST_ARGLST 41
#define AST_ARGLSTEND 42
#define AST_VEC 43
#define AST_FUNC 44


typedef struct ast_node
{
    int type;
    HASH_NODE* symbol;
    struct ast_node* son[MAX_SONS];
} AST;

AST* astCreate(int type, HASH_NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3);
void astPrint(AST* node, int level);
void uncompile(AST* node, FILE* output);

#endif
//END