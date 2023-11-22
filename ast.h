#include "hash.h"

#define MAX_SONS 4

#define AST_SYMBOL 0
#define AST_ADD 0
#define AST_SUB 0

typedef struct ast_node
{
    int type;
    HASH_NODE* symbol;
    struct ast_node* son[MAX_SONS];
} AST;

AST* astCreate(int type, HASH_NODE* symbol,AST* son0, AST* son1, AST* son2, AST* son3);
void astPrint(AST* node, int level);

//END