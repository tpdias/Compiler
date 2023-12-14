//Thiago Parisotto Dias
#include "semantic.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

int semanticErrors = 0;

void check_undeclared(AST *node){
    semanticErrors += checkUndeclaredHash();
}

void check_and_set_declarations(AST *node){
    if(node == NULL) {
        return;
    }
    switch(node->type){
        case AST_DECVAR:
            //check if it is arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Variable %s already declared\n", node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_VAR;
            }
            break;
        case AST_DECVEC:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Vector %s already declared\n", node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_VEC;
            }
            break;
        case AST_DECFUNC:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Function %s already declared\n", node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_FUN;
            }
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_and_set_declarations(node->son[i]);
    }
}