#include "ast.h"

AST* astCreate(int type, HASH_NODE* symbol,AST* son0, AST* son1, AST* son2, AST* son3) {
    AST* newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;
    return newnode;
}


//print all levels of the ast tree based on the current node, and give it spaces for each level
void astprint(AST* node, int level) {
    if (node == 0) return;
    int i;
    for (i = 0; i < level; ++i) fprintf(stderr,"  ");
    printf("AST(");
    switch (node->type) {
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
        case AST_ADD: fprintf(stderr, "AST_ADD"); break;
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        default: fprintf(stderr, "UNKNOWN"); break;
    }
    fprintf(stderr,", %s\n", node->symbol?node->symbol->text:"");
    for(i = 0; i < MAX_SONS; ++i) astprint(node->son[i], level+1);
}