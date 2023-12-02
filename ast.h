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

typedef struct ast_node
{
    int type;
    HASH_NODE* symbol;
    struct ast_node* son[MAX_SONS];
} AST;

AST* astCreate(int type, HASH_NODE* symbol,AST* son0, AST* son1, AST* son2, AST* son3);
void astPrint(AST* node, int level);

#endif
//END